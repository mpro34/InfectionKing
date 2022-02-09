#include "ShaderProgram.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

ShaderProgram::ShaderProgram() :
	m_handle(0)
{

}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_handle);
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
    checkCompileErrors(vs, ShaderType::VERTEX);

    // Create, assign, and compile the fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSourcePtr, nullptr);
    glCompileShader(fs);
    checkCompileErrors(fs, ShaderType::FRAGMENT);

    // Create the shader program with all compiled shaders above (vertex and fragment)
    m_handle = glCreateProgram();
    glAttachShader(m_handle, vs);
    glAttachShader(m_handle, fs);
    glLinkProgram(m_handle);
    checkCompileErrors(m_handle, ShaderType::PROGRAM);

    // Once shader program is created, we can free the shader memory before entering the game loop.
    glDeleteShader(vs);
    glDeleteShader(fs);

    m_uniform_locations.clear();

	return true;
}

void ShaderProgram::use()
{
	if (m_handle > 0)
	{
		glUseProgram(m_handle);
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
    if (type == ShaderType::PROGRAM)
    {
        glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint length = 0;
            glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &length);
            std::string errorLog(length, ' ');
            glGetProgramInfoLog(m_handle, length, &length, &errorLog[0]);
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

GLuint ShaderProgram::getProgram() const
{
    return m_handle;
}

GLint ShaderProgram::getUniformLocation(const GLchar* name)
{
    std::map<std::string, GLint>::iterator it = m_uniform_locations.find(name);

    if (it == m_uniform_locations.end())
    {
        m_uniform_locations[name] = glGetUniformLocation(m_handle, name);
    }

    return m_uniform_locations[name];
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v)
{
    GLint loc = getUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v)
{
    GLint loc = getUniformLocation(name);
    glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v)
{
    GLint loc = getUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m)
{
    GLint loc = getUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void ShaderProgram::setUniform(const GLchar* name, const GLfloat f)
{
    GLint loc = getUniformLocation(name);
    glUniform1f(loc, f);
}

void ShaderProgram::setUniform(const GLchar* name, const GLint v)
{
    GLint loc = getUniformLocation(name);
    glUniform1i(loc, v);
}

// Set active texture and pass uniform int to shader
void ShaderProgram::setUniformSampler(const GLchar* name, const GLint& slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    GLint loc = getUniformLocation(name);
    glUniform1i(loc, slot);
}

