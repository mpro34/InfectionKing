#pragma once

#include <GL/glew.h> // OpenGL
#include <GLM/glm.hpp>
#include <string>
#include <map>

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

	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);

private:
	std::string fileToString(const std::string& filename);
	void checkCompileErrors(GLuint shader, ShaderType type);
	GLint getUniformLocation(const GLchar* name);
	
	GLuint m_Handle;
	std::map<std::string, GLint> m_UniformLocations;
};

