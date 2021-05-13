#include "planetglwidget.h"

void PlanetGLWidget::line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
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
}

void PlanetGLWidget::drawCircle(GLfloat cx, GLfloat cy, GLfloat r, GLuint textureID, double angle, GLfloat color_r, GLfloat color_g, GLfloat color_b, bool fill)
{
    constexpr int prec = 50;

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
        verx[stride + stride * i + 8] = std::cos(2 * M_PI * i / prec + angle) / 2 + 0.5f;
        verx[stride + stride * i + 9] = -std::sin(2 * M_PI * i / prec + angle) / 2 + 0.5f;
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

void PlanetGLWidget::drawBackground(GLuint textureID)
{
    GLuint vertexbuffer;
    GLfloat verx[40];

    for (int i = 0; i < 4; i++) {
        verx[i * 10 + 2] = 0;
        verx[i * 10 + 3] = 1;

        verx[i * 10 + 5] = 0;
        verx[i * 10 + 6] = 0;
        verx[i * 10 + 7] = 0;
    }

    verx[0] = -1.f;
    verx[1] = 1.f;
    verx[8] = 0.f;
    verx[9] = 0.f;

    verx[10] = 1.f;
    verx[11] = 1.f;
    verx[18] = 1.f;
    verx[19] = 0.f;

    verx[20] = -1.f;
    verx[21] = -1.f;
    verx[28] = 0.f;
    verx[29] = 1.f;

    verx[30] = 1.f;
    verx[31] = -1.f;
    verx[38] = 1.f;
    verx[39] = 1.f;




    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    glBufferData(GL_ARRAY_BUFFER, 40 * sizeof(GLfloat), verx, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void *) 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void *) (4 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (void *) (8 * sizeof(GLfloat)));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);



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
