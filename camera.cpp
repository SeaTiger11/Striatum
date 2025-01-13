#include "camera.h"

//Constructor
Camera::Camera(int width, int height) {
	Camera::width = width;
	Camera::height = height;
}

//Sets camera position and orientation
VertexUniformBufferObject Camera::GetUpdatedMatrix(float FOVdeg, float nearPlane, float farPlane) {
	vubo.view = glm::lookAt(position, position + orientation, up);
	vubo.proj = glm::perspective(glm::radians(FOVdeg), float(width / height), nearPlane, farPlane);
	vubo.proj[1][1] *= -1;

	return vubo;
}

//Updates inputs for the camera
void Camera::Inputs(GLFWwindow* window, float deltaTime) {
	//Sets the game to be active (allows inputs)
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !active) {
		active = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetCursorPos(window, width / 2.0f, height / 2.0f);
	}
	//Sets the game to be deactive (disables inputs)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && active) {
		active = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	//All main inputs for the game
	if (active) {
		//Standard WASD movement with velocity based movement
		glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {;
			acceleration += glm::normalize(glm::vec3(orientation.x, orientation.y, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			acceleration += glm::normalize(glm::vec3(-orientation.y, orientation.x, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			acceleration -= glm::normalize(glm::vec3(orientation.x, orientation.y, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			acceleration -= glm::normalize(glm::vec3(-orientation.y, orientation.x, 0));
		}
		//Sprint
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			speed = baseSpeed * sprintMultiplier;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
			speed = baseSpeed;
		}
		//Up and Down
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			acceleration += glm::vec3(0, 0, 1.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			acceleration -= glm::vec3(0, 0, 1.0f);
		}

		if (acceleration != glm::vec3(0.0f, 0.0f, 0.0f))
			velocity += glm::normalize(acceleration) * speed * deltaTime;
		velocity *= friction;
		position += velocity;

		//Gets current mouse position
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		//Centers mouse position and normalizes them based on the screen size before adjusting them based on the sensitivity
		float rotX = sensitivity * float(mouseY - (height / 2.0f)) / height;
		float rotY = sensitivity * float(mouseX - (width / 2.0f)) / width;

		//Finds the new camera orientation
		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

		//Ensures the new orientation isn't overrotated (prevents backflips/frontflips with the camera) and if not, updates camera angle
		if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
			orientation = glm::rotate(newOrientation, glm::radians(-rotY), up);
		}

		//Resetting the mouse to the center of the screen
		glfwSetCursorPos(window, width / 2.0f, height / 2.0f);
	}
}