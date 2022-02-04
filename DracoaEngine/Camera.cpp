#include "Camera.hpp"

// Default camera values
const float DEF_FOV = 45.0f;

Camera::Camera() :
	m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_target_pos(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_right(glm::vec3(0.0f, 0.0f, 0.0f)),
	WORLD_UP(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_yaw(glm::pi<float>()),
	m_pitch(0.0f),
	m_FOV(DEF_FOV)
{

}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_position, m_target_pos, m_up);
}

const glm::vec3& Camera::getLook() const
{
	return m_look;
}

const glm::vec3& Camera::getRight() const
{
	return m_right;
}

const glm::vec3& Camera::getUp() const
{
	return m_up;
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

/* FPS Camera Class */
FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch)
{
	m_position = position;
	m_yaw = yaw;
	m_pitch = pitch;
}

void FPSCamera::setPosition(const glm::vec3& position)
{
	m_position = position;
}

// Rotate camera using Yaw and Pitch angles passed in degrees
void FPSCamera::rotate(float yaw, float pitch)
{
	m_yaw += glm::radians(yaw);
	m_pitch += glm::radians(pitch);

	// Constrain the pitch
	m_pitch = glm::clamp(m_pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	updateCameraVectors();
}
void FPSCamera::move(const glm::vec3& offsetPos)
{
	m_position += offsetPos;
	updateCameraVectors();
}

// Calculates the vectors from the Camera's (updated) Euler Angles
void FPSCamera::updateCameraVectors()
{
	// Spherical to Cartesian Coordinates
	glm::vec3 look;
	look.x = cosf(m_pitch) * sinf(m_yaw);
	look.y = sinf(m_pitch);
	look.z = cosf(m_pitch) * cosf(m_yaw);

	m_look = glm::normalize(look);

	// Re-calc the Right and Up vectors.
	m_right = glm::normalize(glm::cross(m_look, WORLD_UP));
	m_up = glm::normalize(glm::cross(m_right, m_look));

	m_target_pos = m_position + m_look;
}