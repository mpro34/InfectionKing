#pragma once

#include <GL/glew.h> // OpenGL
#include <string>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	enum ShaderType
	{
		VERTEX = 0,
		FRAGMENT,
		PROGRAM
	};

	bool loadShaders(const char* vsFilesname, const char* fsFilename);
	void use();

private:
	std::string fileToString(const std::string& filename);
	void checkCompileErrors(GLuint shader, ShaderType type);
	GLuint m_Handle;
};

