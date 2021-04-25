#ifndef PLANETGLWIDGET_H
#define PLANETGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QObject>
#include <QWidget>
#include <QTimer>

#include <chrono>

class PlanetGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

private:
    GLbyte *mocanu;
    GLsizei mocanuw;
    GLsizei mocanuh;

    GLuint programID;
    GLuint textureID;

    int i;
    std::chrono::high_resolution_clock clock;
public:
    PlanetGLWidget(QWidget *parent = nullptr);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void loadShaders();
    void loadShader(const char *shader, GLuint shaderID);
    void loadTextures();

    void drawCircle(GLfloat cx, GLfloat cy, GLfloat r, GLfloat color_r = 0.0f, GLfloat color_g = 0.0f, GLfloat color_b = 0.0f, bool fill = true);
    //void line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    //void circlev2(GLfloat cx, GLfloat cy, GLfloat r);
};

#endif // PLANETGLWIDGET_H
