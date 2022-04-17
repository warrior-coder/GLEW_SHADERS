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

	// очистка комментариев кода
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


	// каждый объект OpenGL имеет свой id
	GLuint vertexCoordinatesBufferId; // номер буфера массива координат вершин
	GLuint vertexColorsBufferId; // номер буфера массива цветов вершин

	glGenBuffers(1, &vertexCoordinatesBufferId); // генерируем 1 буфер на видеокарте
	glBindBuffer(GL_ARRAY_BUFFER, vertexCoordinatesBufferId); // связываем vertexCoordinatesBufferId с буфером
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoordinates), vertexCoordinates, GL_STATIC_DRAW); // копируем данные vertexCoordinates из оперативной памяти в буфер на видеокарту
	glBindBuffer(GL_ARRAY_BUFFER, 0u); // отвязываемся от буфера

	// создадим и заполним буфер цветов вершин
	glGenBuffers(1, &vertexColorsBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorsBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0u);


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
			glBindBuffer(GL_ARRAY_BUFFER, vertexCoordinatesBufferId);
			glVertexPointer(2, GL_FLOAT, 0, NULL); // источник вершин NULL значит координаты вершин берутся из текущего буфера
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, vertexColorsBufferId);
			glColorPointer(3, GL_FLOAT, 0, NULL); // источник вершин NULL значит цвета вершин берутся из текущего буфера
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnableClientState(GL_VERTEX_ARRAY); // включаем использование массива вершин
			glEnableClientState(GL_COLOR_ARRAY); // включаем использование массива цветов
				glDrawArrays(GL_TRIANGLES, 0, 3); // рисуем примитив по включенному массиву
			glDisableClientState(GL_VERTEX_ARRAY); // отключаем использование массива вершин
			glDisableClientState(GL_COLOR_ARRAY); // отключаем использование массива цветов
		glUseProgram(0u); // отключаем программу отрисовки

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