#pragma once

#include <vector>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLM/glm.hpp>


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal; // used for lighting
	glm::vec2 texCoords;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool loadOBJ(const std::string& filename);
	void draw();

private:
	void initBuffers();

	bool m_loaded;
	std::vector<Vertex> m_vertices;
	GLuint m_vbo, m_vao;
};

