#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

#include "GameInfo.h"
#include "Input.h"

class Camera
{
public:
	Camera()
		: myPosition(0, 5, -1)
		, myRotation(0.0, 0, 0)
	{

	}

	void Move()
	{
		float speed = 5.0f;

		float newSpeed = speed;

		if (Input::IsDown(GLFW_KEY_LEFT_SHIFT))
			newSpeed *= 25.0f;


		if (Input::IsDown(GLFW_KEY_UP) || Input::IsDown(GLFW_KEY_W))
			myPosition.z -= GameInfo::ourDeltaTime * newSpeed;

		if (Input::IsDown(GLFW_KEY_DOWN) || Input::IsDown(GLFW_KEY_S))
			myPosition.z += GameInfo::ourDeltaTime * newSpeed;

		if (Input::IsDown(GLFW_KEY_A))
			myPosition.x -= GameInfo::ourDeltaTime * newSpeed;

		if (Input::IsDown(GLFW_KEY_D))
			myPosition.x += GameInfo::ourDeltaTime * newSpeed;

		if (Input::IsDown(GLFW_KEY_Q))
			myPosition.y += GameInfo::ourDeltaTime * newSpeed;

		if (Input::IsDown(GLFW_KEY_Z))
			myPosition.y -= GameInfo::ourDeltaTime * newSpeed;
		
		if (Input::IsDown(GLFW_KEY_E))
			myRotation.x -= GameInfo::ourDeltaTime * newSpeed * 0.01f;
		
		if (Input::IsDown(GLFW_KEY_C))
			myRotation.x += GameInfo::ourDeltaTime * newSpeed * 0.01f;

		if (Input::IsDown(GLFW_KEY_LEFT))
			myRotation.y -= GameInfo::ourDeltaTime * newSpeed * 0.01f;

		if (Input::IsDown(GLFW_KEY_RIGHT))
			myRotation.y += GameInfo::ourDeltaTime * newSpeed * 0.01f;


		//myRotation.y -= GameInfo::ourDeltaTime * newSpeed;
	}

	void InvertCamera()
	{
		myRotation.x = -myRotation.x;
	}

	vec3 myPosition;
	vec3 myRotation;

private:
};
