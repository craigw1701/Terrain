#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

#include "GameInfo.h"

class Camera
{
public:
	Camera(GLFWwindow& aWindow)
		: myWindow(aWindow)
		, myPosition(0, 5, -1)
		, myRotation(0.0, 0, 0)
	{

	}

	void Move()
	{
		float speed = 1.0f;

		float newSpeed = speed;

		if (glfwGetKey(&myWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			newSpeed *= 5.0f;
		}

		if (glfwGetKey(&myWindow, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(&myWindow, GLFW_KEY_W) == GLFW_PRESS) {
			myPosition.z -= GameInfo::ourDeltaTime * newSpeed;
		}
		if (glfwGetKey(&myWindow, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(&myWindow, GLFW_KEY_S) == GLFW_PRESS) {
			myPosition.z += GameInfo::ourDeltaTime * newSpeed;
		}
		if (glfwGetKey(&myWindow, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(&myWindow, GLFW_KEY_A) == GLFW_PRESS) {
			myPosition.x -= GameInfo::ourDeltaTime * newSpeed;
		}
		if (glfwGetKey(&myWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(&myWindow, GLFW_KEY_D) == GLFW_PRESS) {
			myPosition.x += GameInfo::ourDeltaTime * newSpeed;
		}
		if (glfwGetKey(&myWindow, GLFW_KEY_Q) == GLFW_PRESS) {
			myPosition.y += GameInfo::ourDeltaTime * newSpeed;
		}
		if (glfwGetKey(&myWindow, GLFW_KEY_Z) == GLFW_PRESS) {
			myPosition.y -= GameInfo::ourDeltaTime * newSpeed;
		}

		//myRotation.y -= GameInfo::ourDeltaTime * newSpeed;
	}

	vec3 myPosition;
	vec3 myRotation;

private:
	GLFWwindow& myWindow;
};
