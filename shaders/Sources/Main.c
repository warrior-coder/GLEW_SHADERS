#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Loaders.h"


/*
Shader – программа, которая выполняется на видеокарте
Vertex Shader выполняется для каждой вершины примитива
Fragment Shader выполняется для каждого пикселя примитива
.glsl – OpenGL Shading Language (язык программирования шейдеров)
 */


int main(void)
{
	// инициализация GLFW
	if (!glfwInit())
	{
		return 1;
	}


	// создание окна с контекстом OpenGL
	GLFWwindow* window = glfwCreateWindow(512, 512, "GLEW Shaders", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return 2;
	}
	glfwMakeContextCurrent(window); // делаем контекст окна текущим контекстом отрисовки


	// инициализация GLEW (только после подключения контекста отрисовки OpenGL)
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return 3;
	}


	// загрузка вершинного шейдер
	GLuint fragmentShaderId = LoadVertexShader("Resources/VertexShader.glsl");
	if (!fragmentShaderId)
	{
		glDeleteShader(fragmentShaderId);
		glfwTerminate();
		return 4;
	}


	// загрузка фрагментного шейдера
	GLuint vertexShaderId = LoadFragmentShader("Resources/FragmentShader.glsl");
	if (!vertexShaderId)
	{
		glfwTerminate();
		return 5;
	}


	// создание шейдерной программы
	GLuint programId = CreateShaderProgram(vertexShaderId, fragmentShaderId);
	glDeleteShader(fragmentShaderId); // удаление использованного шейдера
	glDeleteShader(vertexShaderId);
	if (!programId)
	{
		glfwTerminate();
		return 6;
	}


	// создание буфера координат вершин
	const GLfloat vertexCoordinates[] = {
		0.0f, 1.0f,
		0.87f, -0.5f,
		-0.87f, -0.5f
	}; // массив координат вершин
	GLuint vertexCoordinatesBufferId = CreateVertexBufferObject(vertexCoordinates, sizeof(vertexCoordinates)); // id буфера координат вершин

	
	// создание буфера цветов вершин
	const GLfloat vertexColors[] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	}; // массив цветов вершин
	GLuint vertexColorsBufferId = CreateVertexBufferObject(vertexColors, sizeof(vertexColors)); // id буфера цветов вершин


	GLfloat rotateAngle = 0.0f;

	// основной цикл программы
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		// отрисовка изображения
		glClearColor(0.5f, 0.5f, 0.5f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glPushMatrix(); // помещение матрицы в стек
		glTranslatef(0, 0, 0); // смешение системы координат
		glRotatef(rotateAngle, 0, 0, 1); // вращение системы координат на rotateAngle вокруг Oz
		rotateAngle += 0.05f;

		glUseProgram(programId); // активация программы отрисовки
			glBindBuffer(GL_ARRAY_BUFFER, vertexCoordinatesBufferId);
				glVertexPointer(2, GL_FLOAT, 0, NULL); // источник вершин NULL значит координаты вершин берутся из текущего буфера
			glBindBuffer(GL_ARRAY_BUFFER, 0u);
			glBindBuffer(GL_ARRAY_BUFFER, vertexColorsBufferId);
				glColorPointer(3, GL_FLOAT, 0, NULL); // источник вершин NULL значит цвета вершин берутся из текущего буфера
			glBindBuffer(GL_ARRAY_BUFFER, 0u);

			glEnableClientState(GL_VERTEX_ARRAY); // включение использования массива вершин
			glEnableClientState(GL_COLOR_ARRAY); // включение использования массива цветов
				glDrawArrays(GL_TRIANGLES, 0, 3); // отрисовка примитива по включенному массиву (3 вершины начиная с индекса 0)
			glDisableClientState(GL_VERTEX_ARRAY); // отключение использование массива вершин
			glDisableClientState(GL_COLOR_ARRAY); // отключение использование массива цветов
		glUseProgram(0u); // отключение программы отрисовки

		glPopMatrix(); // загрузка матрицы из стека (возвращение ее прежнего состояния)

		// обмен переднего и заднего буферов
		glfwSwapBuffers(window);

		// запрос на обработку событий
		glfwPollEvents();
	}


	// освобождение памяти
	glDeleteProgram(programId);
	glfwTerminate();

	return 0;
}