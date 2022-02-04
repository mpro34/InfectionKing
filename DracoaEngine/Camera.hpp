#pragma once

#include <GLM/glm.hpp>

// Abstract Camera Class
class Camera
{
public:
	glm::mat4 getViewMatrix() const;
	virtual void rotate(float yaw, float pitch) {} // in degrees

protected:
	Camera();

	glm::vec3 m_position;
	glm::vec3 m_target_pos;
	glm::vec3 m_up;

	// Eulers angles (in radians)
	float m_yaw;
	float m_pitch;
};


class OrbitCamera : public Camera
{
public:
	OrbitCamera();

	virtual void rotate(float yaw, float pitch); // in degrees

	void setLookAt(const glm::vec3& target);
	void setRadius(float radius);

private:
	void updateCameraVectors();
	float m_radius;
};
