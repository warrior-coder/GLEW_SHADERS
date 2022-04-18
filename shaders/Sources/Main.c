#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Loaders.h"


/*
Shader � ���������, ������� ����������� �� ����������
Vertex Shader ����������� ��� ������ ������� ���������
Fragment Shader ����������� ��� ������� ������� ���������
.glsl � OpenGL Shading Language (���� ���������������� ��������)
 */


int main(void)
{
	// ������������� GLFW
	if (!glfwInit())
	{
		return 1;
	}


	// �������� ���� � ���������� OpenGL
	GLFWwindow* window = glfwCreateWindow(512, 512, "GLEW Shaders", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return 2;
	}
	glfwMakeContextCurrent(window); // ������ �������� ���� ������� ���������� ���������


	// ������������� GLEW (������ ����� ����������� ��������� ��������� OpenGL)
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return 3;
	}


	// �������� ���������� ������
	GLuint fragmentShaderId = LoadVertexShader("Resources/VertexShader.glsl");
	if (!fragmentShaderId)
	{
		glDeleteShader(fragmentShaderId);
		glfwTerminate();
		return 4;
	}


	// �������� ������������ �������
	GLuint vertexShaderId = LoadFragmentShader("Resources/FragmentShader.glsl");
	if (!vertexShaderId)
	{
		glfwTerminate();
		return 5;
	}


	// �������� ��������� ���������
	GLuint programId = CreateShaderProgram(vertexShaderId, fragmentShaderId);
	glDeleteShader(fragmentShaderId); // �������� ��������������� �������
	glDeleteShader(vertexShaderId);
	if (!programId)
	{
		glfwTerminate();
		return 6;
	}


	// �������� ������ ��������� ������
	const GLfloat vertexCoordinates[] = {
		0.0f, 1.0f,
		0.87f, -0.5f,
		-0.87f, -0.5f
	}; // ������ ��������� ������
	GLuint vertexCoordinatesBufferId = CreateVertexBufferObject(vertexCoordinates, sizeof(vertexCoordinates)); // id ������ ��������� ������

	
	// �������� ������ ������ ������
	const GLfloat vertexColors[] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	}; // ������ ������ ������
	GLuint vertexColorsBufferId = CreateVertexBufferObject(vertexColors, sizeof(vertexColors)); // id ������ ������ ������


	GLfloat rotateAngle = 0.0f;

	// �������� ���� ���������
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		// ��������� �����������
		glClearColor(0.5f, 0.5f, 0.5f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glPushMatrix(); // ��������� ������� � ����
		glTranslatef(0, 0, 0); // �������� ������� ���������
		glRotatef(rotateAngle, 0, 0, 1); // �������� ������� ��������� �� rotateAngle ������ Oz
		rotateAngle += 0.05f;

		glUseProgram(programId); // ��������� ��������� ���������
			glBindBuffer(GL_ARRAY_BUFFER, vertexCoordinatesBufferId);
				glVertexPointer(2, GL_FLOAT, 0, NULL); // �������� ������ NULL ������ ���������� ������ ������� �� �������� ������
			glBindBuffer(GL_ARRAY_BUFFER, 0u);
			glBindBuffer(GL_ARRAY_BUFFER, vertexColorsBufferId);
				glColorPointer(3, GL_FLOAT, 0, NULL); // �������� ������ NULL ������ ����� ������ ������� �� �������� ������
			glBindBuffer(GL_ARRAY_BUFFER, 0u);

			glEnableClientState(GL_VERTEX_ARRAY); // ��������� ������������� ������� ������
			glEnableClientState(GL_COLOR_ARRAY); // ��������� ������������� ������� ������
				glDrawArrays(GL_TRIANGLES, 0, 3); // ��������� ��������� �� ����������� ������� (3 ������� ������� � ������� 0)
			glDisableClientState(GL_VERTEX_ARRAY); // ���������� ������������� ������� ������
			glDisableClientState(GL_COLOR_ARRAY); // ���������� ������������� ������� ������
		glUseProgram(0u); // ���������� ��������� ���������

		glPopMatrix(); // �������� ������� �� ����� (����������� �� �������� ���������)

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