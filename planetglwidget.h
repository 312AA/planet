#ifndef PLANETGLWIDGET_H
#define PLANETGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QObject>
#include <QWidget>
#include <QTimer>

class PlanetGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

private:
    GLuint programID;
    int i;
public:
    PlanetGLWidget(QWidget *parent = nullptr);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void loadShaders();
    void drawCircle(GLfloat cx, GLfloat cy, GLfloat r);
    void line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    void circlev2(GLfloat cx, GLfloat cy, GLfloat r);
};

#endif // PLANETGLWIDGET_H
