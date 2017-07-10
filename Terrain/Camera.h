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
#include "Player.h"

class Camera
{
public:
	Camera(Player& aPlayer)
		: myPlayer(aPlayer)
		, myPosition(0, 0, 0)
		, myRotation(20, 0, 0)
	{
	}

	void Update()
	{
		CalculateZoon();
		CalculatePitch();
		CalculateAngleAroundPlayer();
		float horizontalDistance = CalculateHorizontalDistance();
		float verticalDistance = CalculateVerticalDistance();

		CalculateCameraPosition(horizontalDistance, verticalDistance);
		myRotation.y = 180 - (myPlayer.myRotation.y + myAngleAroundPlayer);

		if (!GameInfo::ourFlyCamera)
			return;

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

private:
	void CalculateCameraPosition(float aHorizontalDistance, float aVerticalDistance)
	{
		float theta = myPlayer.myRotation.y + myAngleAroundPlayer;
		float offsetX = aHorizontalDistance * sin(radians(theta));
		float offsetZ = aHorizontalDistance * cos(radians(theta));
		myPosition.x = myPlayer.myPosition.x - offsetX;
		myPosition.z = myPlayer.myPosition.z - offsetZ;
		myPosition.y = myPlayer.myPosition.y + aVerticalDistance;
	}

	float CalculateHorizontalDistance()
	{
		return myDistanceFromPlayer * cos(radians(myRotation.x));
	}

	float CalculateVerticalDistance()
	{
		return myDistanceFromPlayer * sin(radians(myRotation.x));
	}

	void CalculateZoon()
	{
		myDistanceFromPlayer = clamp(myDistanceFromPlayer - Input::GetScrollDelta().y * 10.0f, 20.0f, 1000.0f);
	}

	void CalculatePitch()
	{
		if (Input::IsButtonDown(GLFW_MOUSE_BUTTON_2))
		{
			myRotation.x = clamp(myRotation.x - Input::MousePosDelta().y / 10.0f, -90.0f, 90.0f);
			
		}
	}

	void CalculateAngleAroundPlayer()
	{
		if (Input::IsButtonDown(GLFW_MOUSE_BUTTON_1))
		{
			myAngleAroundPlayer -= Input::MousePosDelta().x / 10.0f;
		}
	}
	
	Player& myPlayer;
	float myDistanceFromPlayer = 50.0f;
	float myAngleAroundPlayer = 0.0f;
public:
	vec3 myPosition;
	vec3 myRotation;
	//float myPitch = 20.0f;
	//float myYaw = 0.0f;
	//float myRoll = 0.0f;

private:
};
