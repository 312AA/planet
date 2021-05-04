#include "planetglwidget.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void PlanetGLWidget::initializeGL()
{
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
    glGenTextures(1, &textureID1);
    glBindTexture(GL_TEXTURE_2D, textureID1);
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

    glGenTextures(1, &textureID2);
    glBindTexture(GL_TEXTURE_2D, textureID2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    data = stbi_load("C:\\Users\\Vlad\\Desktop\\planet\\planet\\mocanu.jpeg", &width, &height, &nrChannels, 0);
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
