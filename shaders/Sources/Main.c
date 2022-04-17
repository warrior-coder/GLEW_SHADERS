#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

/*
Shader – программа, которая выполняется на видеокарте
Vertex Shader выполняется для каждой вершины примитива
Fragment Shader выполняется для каждого пикселя примитива
.glsl – OpenGL Shading Language (язык программирования шейдеров)
 */

 // загрузка кода шейдера в С-строку из файла
char* LoadShader(const char* fileName)
{
	FILE* filePtr;

	// открытие файла
	fopen_s(&filePtr, fileName, "rb");
	if (!filePtr)
	{
		printf("FILE_OPEN_ERROR: %s\n", fileName);
		return NULL;
	}

	// вычисление размера файла
	fseek(filePtr, 0, SEEK_END);
	int fileSize = ftell(filePtr);
	fseek(filePtr, 0, SEEK_SET);
	printf("File size: %d\n", fileSize);

	// чтение кода из файла
	char* shaderCode = malloc((fileSize + 1) * sizeof(char));
	if (!shaderCode)
	{
		printf("MEMORY_ALLOCATE_ERROR\n");
		return NULL;
	}
	memset(shaderCode, '\0', fileSize + 1);
	fread(shaderCode, sizeof(char), fileSize, filePtr);

	printf("Code size: %d\n", strlen(shaderCode));
	printf("Shader code:\n%s\n", shaderCode);

	return shaderCode;
}

// функция обработки событий нажатия клавиш
void KeyCallback(GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (keyCode == GLFW_KEY_ESCAPE)
		{
			
		}
	}
	else if (action == GLFW_RELEASE)
	{
		
	}
}

int main(void)
{
	// инициализация GLFW
	if (!glfwInit())
	{
		return 1;
	}

	// создание окна с контекстом OpenGL
	GLFWwindow* window = glfwCreateWindow(400, 400, "GLEW Shaders", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return 2;
	}

	// делаем контекст окна текущим контекстом отрисовки
	glfwMakeContextCurrent(window);

	// инициализация GLEW (только после подключения контекста отрисовки OpenGL)
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return 3;
	}


	GLint success;
	GLchar infoLog[512];

	// компиляция вершинного шейдера
	char* vertexShaderCode = LoadShader("Resources/VertexShader.glsl");
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER); // создает Vertex Shader и возвращает его id
	glShaderSource(vertexShaderId, 1, &vertexShaderCode, NULL); // задаем текст кода шейдера
	glCompileShader(vertexShaderId);
	free(vertexShaderCode);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success); // проверка статуса компиляции шейдера
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog); // получаем текст ошибки
		printf("VERTEX_SHADER_COMPILATION_FAILED: %s\n", infoLog);

		// освобождение выделенной памяти
		glDeleteShader(vertexShaderId);
		glfwTerminate();

		return 4;
	}


	// компиляция фрагментного шейдера
	char* fragmentShaderCode = LoadShader("Resources/FragmentShader.glsl");
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); // создает Fragment Shader и возвращает его id
	glShaderSource(fragmentShaderId, 1, &fragmentShaderCode, NULL); // задаем текст кода шейдера
	glCompileShader(fragmentShaderId);
	free(fragmentShaderCode);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success); // проверка статуса компиляции шейдера
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog); // получаем текст ошибки
		printf("FRAGMENT_SHADER_COMPILATION_FAILED: %s\n", infoLog);

		// освобождение выделенной памяти
		glDeleteShader(fragmentShaderId);
		glDeleteShader(vertexShaderId);
		glfwTerminate();

		return 5;
	}


	// создаем и линкуем шейдерную программу
	GLuint programId = glCreateProgram(); // создаем шейдерную программу

	glAttachShader(programId, fragmentShaderId); // присоединяем фрагментный шейдер к программе
	glAttachShader(programId, vertexShaderId); // присоединяем вершинный шейдер к программе

	glLinkProgram(programId); // связываем все шейдеры в программу

	// удаление использованных шейдеров
	glDeleteShader(fragmentShaderId);
	glDeleteShader(vertexShaderId);

	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		printf("SHADER_LINK_FAILED: %s\n", infoLog);

		// освобождение выделенной памяти
		glDeleteProgram(programId);
		glfwTerminate();

		return 6;
	}

	// устанавливаем функцию-обработчик событий окна
	glfwSetKeyCallback(window, KeyCallback);

	// основной цикл программы
	GLfloat rotateAngle = 0.0f;

	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		// отрисовка изображения
		glClearColor(0.5f, 0.5f, 0.5f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glPushMatrix(); // помещаем матрицу в стек
		glTranslatef(0, 0, 0); // смешение системы координат
		glRotatef(rotateAngle, 0, 0, 1); // вращение системы координат на rotateAngle вокруг Oz
		rotateAngle += 0.05f;

		glUseProgram(programId); // активируем программу отрисовки
		glBegin(GL_TRIANGLES);
			glColor3f(1, 0, 0); glVertex2f(0.0f, 1.0f);
			glColor3f(0, 1, 0); glVertex2f(0.87f, -0.5f);
			glColor3f(0, 0, 1); glVertex2f(-0.87f, -0.5f);
		glEnd();
		glUseProgram(0u); // активируем программу отрисовки

		glPopMatrix(); // загружаем матрицу из стека (возвращаем ее прежнее состояние)

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