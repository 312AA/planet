#ifndef PLANETGLWIDGET_H
#define PLANETGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QObject>
#include <QWidget>

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
    void drawCircle(int prec);
};

#endif // PLANETGLWIDGET_H
