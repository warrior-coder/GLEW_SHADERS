#ifndef _LOADERS_H_
#define _LOADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>


char* LoadShaderCode(const char* shaderFilePath); // загрузка кода шейдера в —-строку из файла
GLuint LoadVertexShader(const char* vertexShaderFilePath); // создание и компил€ци€ вершинного шейдера
GLuint LoadFragmentShader(const char* fragmentShaderFilePath); // создание и компил€ци€ фрагментного шейдера
GLuint CreateShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId); // создание и линковка шейдерной программы
GLuint CreateVertexBufferObject(const GLfloat* vertexBufferArray, GLsizei arraySize); // создание объекта вершинного буфера


#endif // !_LOADERS_H_
