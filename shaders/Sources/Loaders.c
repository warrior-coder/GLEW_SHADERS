#include "Loaders.h"


char* LoadShaderCode(const char* shaderFilePath)
{
	FILE* filePtr;
	
	// открытие файла
	fopen_s(&filePtr, shaderFilePath, "rb");
	if (!filePtr)
	{
		printf("FILE_OPEN_ERROR: %s\n", shaderFilePath);
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

GLuint LoadVertexShader(const char* vertexShaderFilePath)
{
	GLchar* vertexShaderCode = LoadShaderCode(vertexShaderFilePath);
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER); // создает Vertex Shader и возвращает его id
	GLint success;

	glShaderSource(vertexShaderId, 1, &vertexShaderCode, NULL); // задаем текст кода шейдера
	glCompileShader(vertexShaderId); // компил€ци€ шейдера
	free(vertexShaderCode);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success); // проверка статуса компил€ции шейдера
	if (!success)
	{
		GLchar infoLog[512];

		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog); // получаем текст ошибки
		printf("VERTEX_SHADER_COMPILATION_FAILED: %s\n", infoLog);

		return 0u;
	}

	return vertexShaderId;
}

GLuint LoadFragmentShader(const char* fragmentShaderFilePath)
{
	GLchar* fragmentShaderCode = LoadShaderCode(fragmentShaderFilePath);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); // создает Fragment Shader и возвращает его id
	GLint success;

	glShaderSource(fragmentShaderId, 1, &fragmentShaderCode, NULL); // задаем текст кода шейдера
	glCompileShader(fragmentShaderId); // компил€ци€ шейдера
	free(fragmentShaderCode);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success); // проверка статуса компил€ции шейдера
	if (!success)
	{
		GLchar infoLog[512];

		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog); // получаем текст ошибки
		printf("FRAGMENT_SHADER_COMPILATION_FAILED: %s\n", infoLog);

		return 0u;
	}

	return fragmentShaderId;
}

GLuint CreateShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
{
	// каждый объект OpenGL имеет свой id, по которому программа ссылаетс€ на него
	GLuint programId = glCreateProgram(); // создаем шейдерную программу
	GLint success;

	glAttachShader(programId, vertexShaderId); // присоедин€ем вершинный шейдер к программе
	glAttachShader(programId, fragmentShaderId); // присоедин€ем фрагментный шейдер к программе
	glLinkProgram(programId); // св€зываем все шейдеры в программу

	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];

		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		printf("SHADER_PROGRAM_LINK_FAILED: %s\n", infoLog);

		return 0u;
	}

	return programId;
}

GLuint CreateVertexBufferObject(const GLfloat* vertexBufferArray, GLsizei arraySize)
{
	GLuint vertexBufferObjectId;

	glGenBuffers(1, &vertexBufferObjectId); // генерируем 1 буфер на видеокарте
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectId); // св€зываем vertexCoordinatesBufferId с буфером
		glBufferData(GL_ARRAY_BUFFER, arraySize, vertexBufferArray, GL_STATIC_DRAW); // копируем данные vertexCoordinates из оперативной пам€ти в буфер на видеокарту
	glBindBuffer(GL_ARRAY_BUFFER, 0u); // отв€зываемс€ от буфера

	return vertexBufferObjectId;
}