#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

/*
Shader � ���������, ������� ����������� �� ����������
Vertex Shader ����������� ��� ������ ������� ���������
Fragment Shader ����������� ��� ������� ������� ���������
.glsl � OpenGL Shading Language (���� ���������������� ��������)
 */

 // �������� ���� ������� � �-������ �� �����
char* LoadShader(const char* fileName)
{
    FILE* filePtr;

    // �������� �����
    fopen_s(&filePtr, fileName, "rb");
    if (!filePtr)
    {
        printf("FILE_OPEN_ERROR: %s\n", fileName);
        return NULL;
    }

    // ���������� ������� �����
    fseek(filePtr, 0, SEEK_END);
    int fileSize = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);
    printf("File size: %d\n", fileSize);

    // ������ ���� �� �����
	char* shaderCode = malloc((fileSize + 1) * sizeof(char));
    if (!shaderCode)
    {
        printf("MEMORY_ALLOCATE_ERROR\n");
        return NULL;
    }
    memset(shaderCode, '\0', fileSize + 1);
    fread(shaderCode, sizeof(char), fileSize, filePtr);

	printf("Code size: %d\n", strlen(shaderCode));
	printf("Code:\n%s\n", shaderCode);

    return shaderCode;
}

int main(void)
{
    // ������������� GLFW
    if (!glfwInit())
    {
        return 1;
    }

    // �������� ���� � ���������� OpenGL
    GLFWwindow* window = glfwCreateWindow(400, 400, "GLEW Shaders", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return 2;
    }

    // ������ �������� ���� ������� ���������� ���������
    glfwMakeContextCurrent(window);

    // ������������� GLEW (������ ����� ����������� ��������� ��������� OpenGL)
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return 3;
    }


    GLint success;
    GLchar infoLog[512];

    // ���������� ���������� �������
    char* vertexShaderCode = LoadShader("Resources/VertexShader.glsl");
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER); // ������� Vertex Shader � ���������� ��� id
    glShaderSource(vertexShaderId, 1, &vertexShaderCode, NULL); // ������ ����� ���� �������
    glCompileShader(vertexShaderId);
    free(vertexShaderCode);

    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success); // �������� ������� ���������� �������
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog); // �������� ����� ������
        printf("VERTEX_SHADER_COMPILATION_FAILED: %s\n", infoLog);

        // ������������ ���������� ������
        glDeleteShader(vertexShaderId);
        glfwTerminate();

        return 4;
    }


	// ���������� ������������ �������
    char* fragmentShaderCode = LoadShader("Resources/FragmentShader.glsl");
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); // ������� Fragment Shader � ���������� ��� id
    glShaderSource(fragmentShaderId, 1, &fragmentShaderCode, NULL); // ������ ����� ���� �������
    glCompileShader(fragmentShaderId);
    free(fragmentShaderCode);

    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success); // �������� ������� ���������� �������
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog); // �������� ����� ������
        printf("FRAGMENT_SHADER_COMPILATION_FAILED: %s\n", infoLog);

        // ������������ ���������� ������
        glDeleteShader(fragmentShaderId);
        glDeleteShader(vertexShaderId);
        glfwTerminate();

        return 5;
    }


    // ������� � ������� ��������� ���������
    GLuint programId = glCreateProgram(); // ������� ��������� ���������

    glAttachShader(programId, fragmentShaderId); // ������������ ����������� ������ � ���������
    glAttachShader(programId, vertexShaderId); // ������������ ��������� ������ � ���������

    glLinkProgram(programId); // ��������� ��� ������� � ���������

    // �������� �������������� ��������
    glDeleteShader(fragmentShaderId);
    glDeleteShader(vertexShaderId);

    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
        printf("SHADER_LINK_FAILED: %s\n", infoLog);

        // ������������ ���������� ������
        glDeleteProgram(programId);
        glfwTerminate();

        return 6;
    }

    glUseProgram(programId); // ���������� ��������� ���������


    // �������� ���� ���������
    while (!glfwWindowShouldClose(window))
    {
		// ��������� �����������
        glClearColor(0.5f, 0.5f, 0.5f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();

        glBegin(GL_TRIANGLES);
	        glColor3f(1, 0, 0); glVertex2f(0, 0.8f);
	        glColor3f(0, 1, 0); glVertex2f(0.87f, -0.7f);
	        glColor3f(0, 0, 1); glVertex2f(-0.87f, -0.7f);
        glEnd();

        glPopMatrix();

        // ����� ��������� � ������� �������
        glfwSwapBuffers(window);

        // ������ �� ��������� �������
        glfwPollEvents();
    }

    // ������������ ������
    glDeleteProgram(programId);
    glfwTerminate();

    return 0;
}