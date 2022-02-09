#pragma once

#include "Mesh.hpp"
#include "Texture2D.hpp"
#include <GLM/glm.hpp>

class Model
{
public:
	Model();
	~Model();

private:
	glm::vec3 m_model_position;
	glm::vec3 m_model_scale;
	Mesh* m_model_mesh = nullptr;
	Texture2D* m_model_texture = nullptr;
};

