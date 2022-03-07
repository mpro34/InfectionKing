#include "Texture2D.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <iostream>

Texture2D::Texture2D() :
	m_texture(0)
{
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_texture);
}

bool Texture2D::loadTexture(const std::string& filename, bool generateMipMaps)
{
	int width, height, components;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &components, STBI_rgb_alpha);
	if (imageData == nullptr)
	{
		std::cerr << "Error loading texture '" << filename << "'" << std::endl;
		return false;
	}

	// Create opengl texture from image data
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	// Convert raw image coordinates and sample to texels (Texel Sampling), xy -> st
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Use linear for texture filtering if too large/small
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Mipmaps have a texture with duplicates of the original image with 1/4 size smaller mipmaps(or duplicates)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	// Generate Mipmap images from original (smaller duplicates)
	if (generateMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(imageData);		 // free the stbi image bytes
	glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture

	return true;
}

void Texture2D::bind(GLuint texUnit)
{
	assert(texUnit >= 0 && texUnit < 32);

	// Bind to the input texture unit; 16+ texture units on a modern graphics card!
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture2D::unbind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0); // Disconnects the texture in the shader
}
