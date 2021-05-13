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
    if (type== 0x8251 || type == 0x8250)
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
    double simframetime = simframetimeSm;
    if(mw->selectedTh) simframetime = simframetimeTH;
    auto t1 = clock.now();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawBackground(textureIDbg);
    glLineWidth(0.1f);

    if(mw->selectedTh) drawCircle(0,0, 0.8*R / maxdistance, textureID1);
    else
    drawCircle(0,0, R / maxdistance, textureID1);

    double dist;
    if (!stopped)
    for (int i = 0; i < simframetime / simticktime; i++) {
        if(bodies.size() == 2 && mw->selectedTh)
            bodies[0].calc(simticktime);
        else
        for (auto &it: bodies) {
                it.calc(simticktime);
        }
    }

    double outx, outy, outvx, outvy, outax, outay, outangle;
    if(mw->selectedTh && bodies.size() == 2)
    {
        double x1 = bodies[0].x;
        double x2 = bodies[1].x;
        double y1 = bodies[0].y;
        double y2 = bodies[1].y;
        double xc = (x1 + x2)/2;
        double yc = (y1 + y2)/2;

        double x1p, x2p, y1p, y2p;

        x1p = xc + amplf * (x1 - xc);
        y1p = yc + amplf * (y1 - yc);
        x2p = xc + amplf * (x2 - xc);
        y2p = yc + amplf * (y2 - yc);

        outx = x1, outy = y1, outvx = bodies[0].vx, outvy = bodies[0].vy, outax = bodies[0].ax, outay = bodies[0].ay;


        drawCircle(x1p / maxdistance, y1p / maxdistance, 0.02, textureID2, bodies[0].angle);
        drawCircle(x2p / maxdistance, y2p / maxdistance, 0.02, textureID2, bodies[1].angle);
        line(x1p / maxdistance, y1p / maxdistance, x2p / maxdistance, y2p / maxdistance);
    }
    else
    for (auto &it: bodies) {
        drawCircle(it.x / maxdistance, it.y / maxdistance, 0.02, textureID2, it.angle);
        outx = it.x, outy = it.y, outvx = it.vx, outvy = it.vy, outax = it.ax, outay = it.ay, outangle = it.angle;
    }

    if (bodies.size() > 0)
        mw->putData(outx, outy, outvx, outvy, outax, outay);

    auto t2 = clock.now();

    std::cerr << "Frame time: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
        << "ms\r"
        << std::flush;
}

void PlanetGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (mw->selectedTh)
        return;

    createBody_x = ( event->position().x() - screensize / 2) * maxdistance / (screensize / 2);
    createBody_y = (-event->position().y() + screensize / 2) * maxdistance / (screensize / 2);
}

void PlanetGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (mw->selectedTh)
        return;

    double x = ( event->position().x() - screensize / 2) * maxdistance / (screensize / 2);
    double y = (-event->position().y() + screensize / 2) * maxdistance / (screensize / 2);

    createSimBody(createBody_x, createBody_y, (createBody_x - x) / 1000, (createBody_y - y) / 1000, mw->selectedRk);
}

void PlanetGLWidget::createSimBody(double x, double y, double vx, double vy, bool rk)
{
    bodies.push_back(Body(body_m, x, y, vx, vy, rk));
}

void PlanetGLWidget::createThBodies(double x, double y, double len, double wcm, double wp, double k, double c)
{
    double dm,dM;
    dm = sqrt(x*x +y*y) - len/2;
    dM = sqrt(x*x +y*y) + len/2;
    double alpha;
    if(x == 0)
    {
        if(y > 0) alpha = PI/2;
        else alpha = 3*PI/2;
    }
    else alpha = atan(y/x);

    if(x < 0) alpha += PI;

    double xb1,yb1, xb2,yb2, v1x,v1y, v2x,v2y;

    xb1 = dm * cos(alpha);
    yb1 = dm * sin(alpha);

    xb2 = dM * cos(alpha);
    yb2 = dM * sin(alpha);

    v1x = -wp * dm * sin(alpha) + wcm * len/2 * sin(alpha);
    v1y = wp * dm * cos(alpha) - wcm * len/2 * cos(alpha);

    v2x = -wp * dm * sin(alpha) - wcm * len/2 * sin(alpha);
    v2y = wp * dm * cos(alpha) + wcm * len/2 * cos(alpha);

    Body b1(body_m, xb1, yb1, v1x, v1y, false, len, k, c);
    Body b2(body_m, xb2, yb2, v2x, v2y, false, len, k, c);

    bodies.push_back(b1);
    bodies.push_back(b2);
    bodies[bodies.size() - 1].tether = &bodies[bodies.size() - 2];
    bodies[bodies.size() - 2].tether = &bodies[bodies.size() - 1];

    mw->angle(alpha*180/PI);
}

void PlanetGLWidget::clear()
{
    bodies.clear();
}

void PlanetGLWidget::resizeGL(int w, int h)
{
    (void) w;
    (void) h;
}
