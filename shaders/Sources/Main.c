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

	// ������� ������������ ����
	for (int i = 0; shaderCode[i] != '\0'; i++)
	{
		if (shaderCode[i] == '/' && shaderCode[i + 1] == '/')
		{
			int j = 0;
			while (shaderCode[i + j] != '\n')
			{
				shaderCode[i + j] = ' ';
				j++;
			}
			i += j;
		}
		if (shaderCode[i] == '/' && shaderCode[i + 1] == '*')
		{
			int j = 0;
			while (shaderCode[i + j] != '*' || shaderCode[i + j + 1] != '/')
			{
				shaderCode[i + j] = ' ';
				j++;
			}
			shaderCode[i + j] = shaderCode[i + j + 1] = ' ';
			j += 2;

			i += j;
		}
	}
	
	printf("Code size: %d\n", strlen(shaderCode));
	printf("Shader code:\n%s\n", shaderCode);

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


	const GLfloat vertexCoordinates[] = {
		0.0f, 1.0f,
		0.87f, -0.5f,
		-0.87f, -0.5f
	};
	const GLfloat vertexColors[] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	};


	// ������ ������ OpenGL ����� ���� id
	GLuint vertexCoordinatesBufferId; // ����� ������ ������� ��������� ������
	GLuint vertexColorsBufferId; // ����� ������ ������� ������ ������

	glGenBuffers(1, &vertexCoordinatesBufferId); // ���������� 1 ����� �� ����������
	glBindBuffer(GL_ARRAY_BUFFER, vertexCoordinatesBufferId); // ��������� vertexCoordinatesBufferId � �������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoordinates), vertexCoordinates, GL_STATIC_DRAW); // �������� ������ vertexCoordinates �� ����������� ������ � ����� �� ����������
	glBindBuffer(GL_ARRAY_BUFFER, 0u); // ������������ �� ������

	// �������� � �������� ����� ������ ������
	glGenBuffers(1, &vertexColorsBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorsBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0u);


	// �������� ���� ���������
	GLfloat rotateAngle = 0.0f;
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		// ��������� �����������
		glClearColor(0.5f, 0.5f, 0.5f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glPushMatrix(); // �������� ������� � ����
		glTranslatef(0, 0, 0); // �������� ������� ���������
		glRotatef(rotateAngle, 0, 0, 1); // �������� ������� ��������� �� rotateAngle ������ Oz
		rotateAngle += 0.05f;

		glUseProgram(programId); // ���������� ��������� ���������
			glBindBuffer(GL_ARRAY_BUFFER, vertexCoordinatesBufferId);
			glVertexPointer(2, GL_FLOAT, 0, NULL); // �������� ������ NULL ������ ���������� ������ ������� �� �������� ������
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, vertexColorsBufferId);
			glColorPointer(3, GL_FLOAT, 0, NULL); // �������� ������ NULL ������ ����� ������ ������� �� �������� ������
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnableClientState(GL_VERTEX_ARRAY); // �������� ������������� ������� ������
			glEnableClientState(GL_COLOR_ARRAY); // �������� ������������� ������� ������
				glDrawArrays(GL_TRIANGLES, 0, 3); // ������ �������� �� ����������� �������
			glDisableClientState(GL_VERTEX_ARRAY); // ��������� ������������� ������� ������
			glDisableClientState(GL_COLOR_ARRAY); // ��������� ������������� ������� ������
		glUseProgram(0u); // ��������� ��������� ���������

		glPopMatrix(); // ��������� ������� �� ����� (���������� �� ������� ���������)

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