#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <iostream>

struct VertexUniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct FragmentUniformBufferObject {
	alignas(16) glm::vec3 camPos;
};

class Camera {
	public:
		//Camera matrices to control position and rotation
		glm::vec3 position = glm::vec3(50.0f, 0.0f, 30.0f);
		//glm::vec3 position = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 orientation = glm::vec3(-1.0f, 0.0f, 0.0f);
		glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		//Screen width and height
		int width, height;

		//Player controller settings
		float baseSpeed = 0.3f, speed = baseSpeed, sprintMultiplier = 3.0f, sensitivity = 100.0f, friction = 0.95f;
		glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

		bool active = false;

		//Constructor
		Camera(int width, int height);

		VertexUniformBufferObject GetUpdatedMatrix(float FOVdeg, float nearPlane, float farPlane);

		//Updates inputs for the camera
		void Inputs(GLFWwindow* window, float deltaTime);
};

#endif