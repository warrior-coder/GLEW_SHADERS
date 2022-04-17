#ifndef _LOADERS_H_
#define _LOADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>


char* LoadShaderCode(const char* shaderFilePath); // �������� ���� ������� � �-������ �� �����
GLuint LoadVertexShader(const char* vertexShaderFilePath); // �������� � ���������� ���������� �������
GLuint LoadFragmentShader(const char* fragmentShaderFilePath); // �������� � ���������� ������������ �������
GLuint CreateShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId); // �������� � �������� ��������� ���������
GLuint CreateVertexBufferObject(const GLfloat* vertexBufferArray, GLsizei arraySize); // �������� ������� ���������� ������


#endif // !_LOADERS_H_
