#pragma once

#include <GLM/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp> // for pi


// ------------------
// Abstract Camera Class
// ------------------

class Camera
{
public:
	glm::mat4 getViewMatrix() const;

	virtual void setPosition(const glm::vec3& position) {}
	virtual void rotate(float yaw, float pitch) {} // in degrees
	virtual void move(const glm::vec3& offsetPos) {}

	const glm::vec3& getLook() const;
	const glm::vec3& getRight() const;
	const glm::vec3& getUp() const;
	const glm::vec3& getPosition() const;

	float getFOV() const { return m_FOV; }
	void setFOV(float fov) { m_FOV = fov; }

protected:
	Camera();

	virtual void updateCameraVectors() {}

	glm::vec3 m_position;
	glm::vec3 m_target_pos;
	glm::vec3 m_up;
	glm::vec3 m_look;
	glm::vec3 m_right;
	const glm::vec3 WORLD_UP;

	// Eulers angles (in radians)
	float m_yaw;
	float m_pitch;

	// Camera parameters
	float m_FOV; // in degrees
};

// ------------------
// Orbit Camera Class
// ------------------

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

// ------------------
// FPS Camera Class
// ------------------

class FPSCamera : public Camera
{
public:
	FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f); // (yaw) initial angle faces -Z [Right Handed]

	virtual void setPosition(const glm::vec3& position);
	virtual void rotate(float yaw, float pitch); // in degrees
	virtual void move(const glm::vec3& offsetPos);

private:
	void updateCameraVectors();
};

// ------------------
// TODO: Create an Isometric view camera
// ------------------

class IsometricCamera : public Camera
{

};
