#pragma once

#include <string>
#include <gl/glew.h>

class Texture2D
{
public:
	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(const std::string& filename, bool generateMipMaps = true);
	void bind(GLuint texUnit = 0);
	void unbind(GLuint texUnit = 0);

private:

	GLuint m_Texture{ 0 };

};

