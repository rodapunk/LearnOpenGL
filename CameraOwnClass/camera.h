#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// defines several possible options for camera movement, used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// an abstract camera class that processes input and calculates the corresponding Euler Angles, vectors and matrices for use in OpenGL
class Camera {
public:
	// camera attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// euler angles
	float yaw;
	float pitch;
	
	// camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	// constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) 
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}

	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}

	// returns the view matrix calculated using Euler Angles and the lookAt matrix
	glm::mat4 getViewMatrix() {
		//return calculate_lookAt_matrix(position, position + front, up);
		return glm::lookAt(position, position + front, up);
	}

	// processes input received from any keyboard-like input system and accepts input parameter in the form of camera ENUM (to abstract it from windowing systems)
	void processKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = movementSpeed * deltaTime;
		if (direction == FORWARD)
			position += front * velocity;
		if (direction == BACKWARD)
			position -= front * velocity;
		if (direction == LEFT)
			position -= right * velocity;
		if (direction == RIGHT)
			position += right * velocity;
		// make sure the user stays at the ground level
		//position.y = 0.0f;
	}

	// processes input received from a mouse input system and expects the offset value in both the x and y directions
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch) {
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		// update front, right and up vectors using the update Euler Angles
		updateCameraVectors();
	}

	// process input received from a mouse scroll-wheel event and only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset) {
		zoom -= (float)yoffset;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 45.0f)
			zoom = 45.0f;
	}

	// custom implementation of the lookAt function
	glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) {
		// 1. position = known
		// 2. calculate cameraDirection
		glm::vec3 zaxis = glm::normalize(position - target);
		// 3. get positive right axis vector
		glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
		// 4. calculate camera up vector
		glm::vec3 yaxis = glm::cross(zaxis, xaxis);

		// create translation and rotation matrix
		// in glm we access elements as mat[col][row] due to column-major layout
		glm::mat4 translation = glm::mat4(1.0f); // identity matrix by default
		translation[3][0] = -position.x; // fourth column, first row
		translation[3][1] = -position.y;
		translation[3][2] = -position.z;
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation[0][0] = xaxis.x; // first column, first row
		rotation[1][0] = xaxis.y;
		rotation[2][0] = xaxis.z;
		rotation[0][1] = yaxis.x; // first column, second row
		rotation[1][1] = yaxis.y;
		rotation[2][1] = yaxis.z;
		rotation[0][2] = zaxis.x; // first column, third row
		rotation[1][2] = zaxis.y;
		rotation[2][2] = zaxis.z;

		// return lookAt matrix as combination of translation and rotation matrix
		return rotation * translation; // remember to read from right to left
	}

private:
	// calculates the front vector from the camera's (updated) Euler Angles
	void updateCameraVectors() {
		// calculate the new front vector
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		this->front = glm::normalize(front);
		// also re-calculate the right and up vector
		this->right = glm::normalize(glm::cross(front, worldUp));
		this->up = glm::normalize(glm::cross(right, front));
	}
};

#endif