#include "planetglwidget.h"

#include <iostream>
#include <string>

#include <cmath>
#include <cstdlib>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char *vertexShader = "#version 330 core\n"
    "layout (location = 0) in vec4 position;\n"
    "layout (location = 1) in vec4 color_in;\n"
    "layout (location = 2) in vec2 texture_in;\n"
    "out vec4 color;\n"
    "out vec2 texture_pos;\n"
    "void main() {\n"
    "    gl_Position = position;\n"
    "    color = color_in;\n"
    "    texture_pos = texture_in;\n"
    "}\n";

const char *fragmentShader = "#version 330 core\n"
    "in vec4 color;\n"
    "in vec2 texture_pos;\n"
    "out vec4 color_out;\n"
    "uniform sampler2D texture_sampler;\n"
    "void main() {\n"
    //"color_out = color;\n"
    "color_out = texture(texture_sampler, texture_pos);\n"
    "}\n";

void /*GLAPIENTRY*/
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
    i = 0;

    //mocanu = (GLbyte *) stbi_load("/home/vgs/src/planet/imagine.jpeg", &mocanuw, &mocanuh, NULL, 0);

    //if (mocanu == nullptr) {
    //    std::cerr << "nu am putut deschide mocanu" << std::endl;
    //    std::perror("");
    //    std::exit(EXIT_FAILURE);
    //}
}

void PlanetGLWidget::initializeGL()
{
    std::setvbuf(stdout, NULL, _IONBF, 0);

    initializeOpenGLFunctions();

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glDebugMessageCallback(MessageCallback, NULL);

    loadShaders();
    loadTextures();

    glUniform1i(glGetUniformLocation(programID, "texture_sampler"), 0);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void PlanetGLWidget::loadShaders()
{
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    loadShader(vertexShader, VertexShaderID);
    loadShader(fragmentShader, FragmentShaderID);


    programID = glCreateProgram();
    glAttachShader(programID, VertexShaderID);
    glAttachShader(programID, FragmentShaderID);
    glLinkProgram(programID);

    GLint result;

    glGetProgramiv(programID, GL_LINK_STATUS, &result);

    if (!result) {
        GLint loglen;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &loglen);

        char *msg = new char[loglen + 1];
        glGetShaderInfoLog(programID, loglen, NULL, msg);
        std::cerr << "Error linking shaders: " << msg << std::endl;
        delete[] msg;

        std::exit(EXIT_FAILURE);
    }

    glDetachShader(programID, VertexShaderID);
    glDetachShader(programID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    glUseProgram(programID);
}

void PlanetGLWidget::loadShader(const char *shader, GLuint shaderID)
{
    glShaderSource(shaderID, 1, &shader, NULL);
    glCompileShader(shaderID);

    GLint result;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    if (!result) {
        GLint loglen;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &loglen);

        char *msg = new char[loglen + 1];
        glGetShaderInfoLog(shaderID, loglen, NULL, msg);
        std::cerr << "Error compiling shader: " << msg << std::endl;
        delete[] msg;

        std::exit(EXIT_FAILURE);
    }
}

void PlanetGLWidget::loadTextures()
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("C:\\Users\\Vlad\\Desktop\\planet\\planet\\mocanu.jpeg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture " << stbi_failure_reason() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    stbi_image_free(data);
}

void PlanetGLWidget::paintGL()
{
    auto t1 = clock.now();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(0.1f);

    //std::printf("%d\n", i++);

    GLfloat xc1,yc1,xc2,yc2,R=0.4f,H=0.7f,l=0.5f,wc=10,wp=10;
    //circlev2(0,0,R);
    drawCircle(0,0,R);
    //line(0,0,R*cos(wc*i*2*M_PI/1000),R*sin(wc*i*2*M_PI/1000));
    //drawCircle(0.7*cos(2*i*M_PI/1000),0.7*sin(2*i*M_PI/1000),0.1);
    xc1=H*cos(wc*i*2*M_PI/1000)+l/2*cos(wp*i*2*M_PI/1000);
    yc1=H*sin(wc*i*2*M_PI/1000)+l/2*sin(wp*i*2*M_PI/1000);
    xc2=H*cos(wc*i*2*M_PI/1000)-l/2*cos(wp*i*2*M_PI/1000);
    yc2=H*sin(wc*i*2*M_PI/1000)-l/2*sin(wp*i*2*M_PI/1000);
    //line(xc1,yc1,xc2,yc2);
    //circlev2(xc1,yc1,0.04);
    drawCircle(xc1,yc1,0.04f, 0, 1, 0, true);
    //circlev2(xc2,yc2,0.04);
    drawCircle(xc2,yc2,0.04f, 0, 0, 1, true);
    //circlev2(0.7*cos(2*i*M_PI/1000),0.7*sin(2*i*M_PI/1000),0.1);

    i++;
    auto t2 = clock.now();




    std::cerr << "Frame time: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
        << "ms"
        << std::endl;
}

/*void PlanetGLWidget::line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    GLuint vertexbuffer;
    GLfloat ptarray[6];
    ptarray[0]=x1;
    ptarray[1]=y1;
    ptarray[2]=0;
    ptarray[3]=x2;
    ptarray[4]=y2;
    ptarray[5]=0;
    glGenBuffers(1,&vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER,6*sizeof(GLfloat),ptarray,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
    glDrawArrays(GL_LINES,0,2);
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vertexbuffer);
}*/

void PlanetGLWidget::drawCircle(GLfloat cx, GLfloat cy, GLfloat r, GLfloat color_r, GLfloat color_g, GLfloat color_b, bool fill)
{
    constexpr int prec = 1000;

    constexpr int stride = 10;
    constexpr int len = (prec + 2) * stride;

    GLfloat verx[len];

    verx[0] = cx;
    verx[1] = cy;
    verx[2] = 0.0f;
    verx[3] = 1.0f;
    verx[4] = color_r;
    verx[5] = color_g;
    verx[6] = color_b;
    verx[7] = 1.0f;
    verx[8] = 0.5f;
    verx[9] = 0.5f;

    for (int i = 0; i < prec; i++) {
        verx[stride + stride * i] = cx + r*std::cos(2 * M_PI * i / prec);
        verx[stride + stride * i + 1] = cy + r*std::sin(2 * M_PI * i / prec);
        verx[stride + stride * i + 2] = 0;
        verx[stride + stride * i + 3] = 1.0f;
        verx[stride + stride * i + 4] = color_r;
        verx[stride + stride * i + 5] = color_g;
        verx[stride + stride * i + 6] = color_b;
        verx[stride + stride * i + 7] = 1.0f;
        verx[stride + stride * i + 8] = std::cos(2 * M_PI * i / prec) / 2 + 0.5f;
        verx[stride + stride * i + 9] = -std::sin(2 * M_PI * i / prec) / 2 + 0.5f;
    }

    verx[len - stride] = verx[stride];
    verx[len - stride + 1] = verx[stride + 1];
    verx[len - stride + 2] = verx[stride + 2];
    verx[len - stride + 3] = verx[stride + 3];
    verx[len - stride + 4] = verx[stride + 4];
    verx[len - stride + 5] = verx[stride + 5];
    verx[len - stride + 6] = verx[stride + 6];
    verx[len - stride + 7] = verx[stride + 7];
    verx[len - stride + 8] = verx[stride + 8];
    verx[len - stride + 9] = verx[stride + 9];

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    glBufferData(GL_ARRAY_BUFFER, len * sizeof(GLfloat), verx, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void *) 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void *) (4 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void *) (8 * sizeof(GLfloat)));


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (fill)
        glDrawArrays(GL_TRIANGLE_FAN, 0, len / stride);
    else
        glDrawArrays(GL_LINE_STRIP, 1, len / stride - 1);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glDeleteBuffers(1, &vertexbuffer);
}

/*void PlanetGLWidget::circlev2(GLfloat cx, GLfloat cy, GLfloat r)
{
    GLfloat x1=cx+r,y1=cy,x2=cx+r,y2=cy;
    for(int i=1;i<1000;i++)
    {
        x1=x2;
        y1=y2;
        x2=cx+r*cos(i*2*M_PI/999);
        y2=cy+r*sin(i*2*M_PI/999);
        line(x1,y1,x2,y2);
    }
}*/

void PlanetGLWidget::resizeGL(int w, int h)
{
    (void) w;
    (void) h;
}
