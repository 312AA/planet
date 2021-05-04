#include "planetglwidget.h"

#include <iostream>
#include <string>

#include <cmath>
#include <cstdlib>
#include <cstdio>

#include <gl/GL.h>

void
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    (void) length;
    (void) source;
    (void) userParam;
    (void) id;

    //if (type != GL_DEBUG_TYPE_ERROR)
    //    return;
    if (type== 0x8251)
            return;
    std::fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
        type, severity, message );
}


PlanetGLWidget::PlanetGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    std::setvbuf(stdout, NULL, _IONBF, 0);
}

void PlanetGLWidget::paintGL()
{
    auto t1 = clock.now();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(0.1f);

    drawCircle(0,0, R / (2 * std::sqrt(2) * maxdistance), textureID2);

    if (!stopped)
    for (auto &it: bodies) {
        for (int i = 0; i < simframetime / simticktime; i++) {
            it.calc(simticktime);
        }
    }

    double outx, outy, outvx, outvy, outax, outay, outangle;
    for (auto &it: bodies) {
        drawCircle(it.x / maxdistance, it.y / maxdistance, 0.1, textureID1, it.angle);
        outx = it.x, outy = it.y, outvx = it.vx, outvy = it.vy, outax = it.ax, outay = it.ay, outangle = it.angle;
    }

    if (bodies.size() > 0)
        mw->putData(outx, outy, outvx, outvy, outax, outay);

    std::cerr << outangle * 180 / PI << " ";

    auto t2 = clock.now();

    std::cerr << "Frame time: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
        << "ms\r"
        << std::flush;
}

void PlanetGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (!createBody)
        return;

    std::cerr << event->position().x() << " " << event->position().y() << std::endl;

    createBody_x = ( event->position().x() - screensize / 2) * maxdistance / (screensize / 2);
    createBody_y = (-event->position().y() + screensize / 2) * maxdistance / (screensize / 2);
}

void PlanetGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!createBody)
        return;

    bool rk = createBodyType == BodyTypeRk;

    std::cerr << event->position().x() << " " << event->position().y() << std::endl;

    double x = ( event->position().x() - screensize / 2) * maxdistance / (screensize / 2);
    double y = (-event->position().y() + screensize / 2) * maxdistance / (screensize / 2);

    bodies.push_back(Body(1000, createBody_x, createBody_y, (createBody_x - x) / 10, (createBody_y - y) / 10, rk));

    createBody = false;
}

void PlanetGLWidget::resizeGL(int w, int h)
{
    (void) w;
    (void) h;
}
