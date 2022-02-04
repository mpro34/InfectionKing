#include "Camera.hpp"
#include <glm/gtx/transform.hpp>

Camera::Camera() :
	m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_target_pos(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_yaw(0.0f),
	m_pitch(0.0f)
{

}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_position, m_target_pos, m_up);
}


/* Orbit Camera */
OrbitCamera::OrbitCamera() :
	m_radius(10.0f)
{
}

void OrbitCamera::setLookAt(const glm::vec3& target)
{
	m_target_pos = target;
}

void OrbitCamera::setRadius(float radius)
{
	m_radius = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::rotate(float yaw, float pitch)
{
	m_yaw = glm::radians(yaw);
	m_pitch = glm::radians(pitch);
	m_pitch = glm::clamp(m_pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	updateCameraVectors();
}

void OrbitCamera::updateCameraVectors()
{
	// Spherical to Cartesian coordinates
	m_position.x = m_target_pos.x + m_radius * cosf(m_pitch) * sinf(m_yaw);
	m_position.y = m_target_pos.y + m_radius * sinf(m_pitch);
	m_position.z = m_target_pos.z + m_radius * cosf(m_pitch) * cosf(m_yaw);
}