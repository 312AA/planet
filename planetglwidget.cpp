#include "planetglwidget.h"

#include <string>

#include <cmath>
#include <cstdlib>
#include <cstdio>

const char *vertexShader = "#version 330 core\n"
    "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
    "void main() {\n"
    "    gl_Position.xyz = vertexPosition_modelspace;\n"
    "    gl_Position.w = 1.0;\n"
    "}\n";

const char *fragmentShader = "#version 330 core\n"
    "out vec3 color;\n"
    "void main() {\n"
    "    color = vec3(1,0,0);\n"
    "}\n";

PlanetGLWidget::PlanetGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    i = 0;
}

void PlanetGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    loadShaders();
    std::setbuf(stdout, NULL);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

void PlanetGLWidget::loadShaders()
{
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(VertexShaderID, 1, &vertexShader, NULL);
    glCompileShader(VertexShaderID);

    GLint result = GL_FALSE;
    GLint InfoLogLength;

    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 0) {
        char *msg = (char *) std::malloc(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, msg);
        std::fprintf(stderr, "%s\n", msg);
        free(msg);

        if (result == GL_FALSE)
            std::exit(EXIT_FAILURE);
    }

    glShaderSource(FragmentShaderID, 1, &fragmentShader, NULL);
    glCompileShader(FragmentShaderID);

    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 0) {
        char *msg = (char *) std::malloc(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, msg);
        std::fprintf(stderr, "%s\n", msg);
        free(msg);

        if (result == GL_FALSE)
            std::exit(EXIT_FAILURE);
    }

    programID = glCreateProgram();
    glAttachShader(programID, VertexShaderID);
    glAttachShader(programID, FragmentShaderID);
    glLinkProgram(programID);

    glGetShaderiv(FragmentShaderID, GL_LINK_STATUS, &result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 0) {
        char *msg = (char *) std::malloc(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, msg);
        std::fprintf(stderr, "%s\n", msg);
        free(msg);

        if (result == GL_FALSE)
            std::exit(EXIT_FAILURE);
    }

    glDetachShader(programID, VertexShaderID);
    glDetachShader(programID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
}

void PlanetGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);
    glLineWidth(3);
    glEnable(GL_LINE_SMOOTH);
    std::printf("%d\n", i++);

    drawCircle(100);
}

void PlanetGLWidget::drawCircle(int prec)
{
    int len = (prec + 2) * 3;
    GLfloat verx[] = {1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f};

    GLuint vertexbuffer;

    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), verx, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_LINES, 0, 2); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

}
/*
void PlanetGLWidget::drawCircle(int prec)
{
    int len = (prec + 2) * 3;
    GLfloat *verx = (GLfloat *) std::malloc(len * sizeof(GLfloat));

    verx[2] = verx[1] = verx[0] = 0.0f;

    for (int i = 0; i < prec; i++) {
        verx[3 + 3 * i] = std::sin(2 * M_PI * i / prec);
        verx[3 + 3 * i + 1] = std::cos(2 * M_PI * i / prec);
        verx[3 + 3 * i + 2] = 0;
    }

    verx[len - 3] = verx[3];
    verx[len - 2] = verx[4];
    verx[len - 1] = 0;

    GLuint vertexbuffer;

    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, len * sizeof(GLfloat), verx, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLE_FAN, 0, len); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    free(verx);
}*/

void PlanetGLWidget::resizeGL(int w, int h)
{
    (void) w;
    (void) h;
}
