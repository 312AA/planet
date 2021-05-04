#ifndef PLANETGLWIDGET_H
#define PLANETGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QPlainTextEdit>

#include <atomic>
#include <chrono>
#include <vector>

#include "body.h"
#include "constants.h"
#include "mainwindow.h"

void
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam );

extern const char *vertexShader;
extern const char *fragmentShader;

enum CreateBodyType {
    BodyTypeEuler,
    BodyTypeRk,
};

class PlanetGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

private:

    GLbyte *mocanu;
    GLsizei mocanuw;
    GLsizei mocanuh;

    GLuint programID;
    GLuint textureID1;
    GLuint textureID2;

    int i;
    std::chrono::high_resolution_clock clock;

    std::vector<Body> bodies;

    double createBody_x;
    double createBody_y;
public:
    MainWindow *mw;

    std::atomic<bool> createBody;
    std::atomic<CreateBodyType> createBodyType;
    std::atomic<bool> stopped;

    PlanetGLWidget(QWidget *parent = nullptr);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void loadShaders();
    void loadShader(const char *shader, GLuint shaderID);
    void loadTextures();

    void drawCircle(GLfloat cx, GLfloat cy, GLfloat r, GLuint textureID, double angle = 0, GLfloat color_r = 0.0f, GLfloat color_g = 0.0f, GLfloat color_b = 0.0f, bool fill = true);
    //void line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    //void circlev2(GLfloat cx, GLfloat cy, GLfloat r);

    void addBodyMouse();
};

#endif // PLANETGLWIDGET_H
