#include "ShaderProgram.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram() :
	m_Handle(0)
{

}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_Handle);
}

bool ShaderProgram::loadShaders(const char* vsFilesname, const char* fsFilename)
{
    std::string vsString = fileToString(vsFilesname);
    std::string fsString = fileToString(fsFilename);
    const GLchar* vsSourcePtr = vsString.c_str();
    const GLchar* fsSourcePtr = fsString.c_str();

    // Create, assign, and compile the vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSourcePtr, nullptr);
    glCompileShader(vs);
    checkCompileErrors(vs, VERTEX);

    // Create, assign, and compile the fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSourcePtr, nullptr);
    glCompileShader(fs);
    checkCompileErrors(fs, FRAGMENT);

    // Create the shader program with all compiled shaders above (vertex and fragment)
    m_Handle = glCreateProgram();
    glAttachShader(m_Handle, vs);
    glAttachShader(m_Handle, fs);
    glLinkProgram(m_Handle);
    checkCompileErrors(m_Handle, PROGRAM);

    // Once shader program is created, we can free the shader memory before entering the game loop.
    glDeleteShader(vs);
    glDeleteShader(fs);

	return true;
}

void ShaderProgram::use()
{
	if (m_Handle > 0)
	{
		glUseProgram(m_Handle);
	}
}

std::string ShaderProgram::fileToString(const std::string& filename)
{
	std::stringstream ss;
	std::ifstream file;

	try
	{
		file.open(filename, std::ios::in);
		if (!file.fail())
		{
			ss << file.rdbuf();
		}

		file.close();
	}
	catch (std::exception ex)
	{
		std::cerr << "Error reading shader filename!" << std::endl;
	}

	return ss.str();
}

void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
    int status = 0;
    if (type == PROGRAM)
    {
        glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint length = 0;
            glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &length);
            std::string errorLog(length, ' ');
            glGetProgramInfoLog(m_Handle, length, &length, &errorLog[0]);
            std::cerr << "Error! Program failed to link. " << errorLog << std::endl;
        }
    }
    else // VERTEX or FRAGMENT
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::string errorLog(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
            std::cerr << "Error! Shader failed to compile. " << errorLog << std::endl;
        }
    }
}