#pragma once

#include "GL.h"

#include "GameInfo.h"
#include "Input.h"
#include "Player.h"
#include "Maths.h"

class Camera : public NonCopyable
{
public:
	Camera(Player& aPlayer)
		: myPlayer(aPlayer)
		, myPosition(0, 0, 0)
		, myRotation(20, 0, 0)
	{
		if (DebugConsole* console = DebugConsole::GetInstance())
		{
			console->AddVariable("Camera.NearZoom", myNearZoom);
			console->AddVariable("Camera.FarZoom", myFarZoom);
			//console->AddVariable("Camera.Position", myPosition);
		}
	}

	vec3 GetF()
	{
		float aHorizontalDistance = cos(radians(myRotation.x));
		float aVerticalDistance = sin(radians(myRotation.x));

		float theta = myAngleAroundPlayer;
		float offsetX = aHorizontalDistance * sin(radians(theta));
		float offsetZ = aHorizontalDistance * cos(radians(theta));

		return vec3(offsetX, -aVerticalDistance, offsetZ);
	}
	vec3 GetR()
	{
		float aHorizontalDistance = cos(radians(myRotation.x));
		float aVerticalDistance = sin(radians(myRotation.x));

		float theta = myAngleAroundPlayer - 90.0f;
		float offsetX = aHorizontalDistance * sin(radians(theta));
		float offsetZ = aHorizontalDistance * cos(radians(theta));

		return vec3(offsetX, -aVerticalDistance, offsetZ);
	}

	void Update(TerrainManager& aTerrainManager)
	{
		if (GameInfo::ourFlyCamera)
		{
			UpdateFlyCamera();
		}
		else
		{
			UpdateThirdPersonCamera(aTerrainManager);
		}
	}

	void InvertCamera()
	{
		myRotation.x = -myRotation.x;
	}

private:
	void UpdateFlyCamera()
	{
		myAngleAroundPlayer += Input::MousePosDelta().x;
		myRotation.x = clamp(myRotation.x - Input::MousePosDelta().y, -90.0f, 90.0f);
		myRotation.y = 180 - (myPlayer.myRotation.y + myAngleAroundPlayer);

		vec3 forward = GetF();
		vec3 right = GetR();
		float speed = (Input::IsDown(GLFW_KEY_LEFT_SHIFT) ? 5.0f : 1.0f) * 100.0f * GameInfo::ourDeltaTime;

		vec3 direction;

		if (Input::IsDown(GLFW_KEY_W))
			direction += forward * speed;
		if (Input::IsDown(GLFW_KEY_S))
			direction -= forward * speed;
		if (Input::IsDown(GLFW_KEY_D))
			direction += right * speed;
		if (Input::IsDown(GLFW_KEY_A))
			direction -= right * speed;

		myPosition += direction;
		return;
	}

	void UpdateThirdPersonCamera(TerrainManager& aTerrainManager)
	{
		CalculateZoon();
		CalculatePitch();
		CalculateAngleAroundPlayer();
		float horizontalDistance = CalculateHorizontalDistance();
		float verticalDistance = CalculateVerticalDistance();

		CalculateCameraPosition(horizontalDistance, verticalDistance);
		float terrainHeight = aTerrainManager.GetHeight(myPosition.x, myPosition.z) + 5.0f;
		if (myPosition.y < terrainHeight)
		{
			myPosition.y = terrainHeight;
		}
		myRotation.y = 180 - (myPlayer.myRotation.y + myAngleAroundPlayer);
	}

	void CalculateCameraPosition(float aHorizontalDistance, float aVerticalDistance)
	{
		myFlyCamPosition = myPlayer.myPosition;
		float theta = myPlayer.myRotation.y + myAngleAroundPlayer;
		float offsetX = aHorizontalDistance * sin(radians(theta));
		float offsetZ = aHorizontalDistance * cos(radians(theta));
		myPosition.x = myPlayer.myPosition.x - offsetX;
		myPosition.z = myPlayer.myPosition.z - offsetZ;
		myPosition.y = myPlayer.myPosition.y + aVerticalDistance;
	}

	glm::mat4 CreateViewMatrix()
	{
		glm::mat4 matrix(1.0f);

		matrix = glm::rotate(matrix, radians(myRotation.x), glm::vec3(1, 0, 0));
		matrix = glm::rotate(matrix, radians(myRotation.y), glm::vec3(0, 1, 0));
		//matrix = glm::rotate(matrix, aCamera.myRotation.z, glm::vec3(0, 0, 1));

		vec3 negativeCameraPos = -myPosition;
		matrix = glm::translate(matrix, negativeCameraPos);
		return matrix;
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
		float speed = Input::IsDown(GLFW_KEY_LEFT_SHIFT) ? 50.0f : 10.0f;
		myDistanceFromPlayer = clamp(myDistanceFromPlayer - Input::GetScrollDelta().y * speed, myNearZoom, myFarZoom);
	}

	void CalculatePitch()
	{
		if (Input::IsMouseButtonDown(GLFW_MOUSE_BUTTON_2))
		{
			float speed = Input::IsDown(GLFW_KEY_LEFT_SHIFT) ? 1.0f : 5.0f;
			myRotation.x = clamp(myRotation.x - Input::MousePosDelta().y / speed, -90.0f, 90.0f);
		}
	}

	void CalculateAngleAroundPlayer()
	{
		if (Input::IsMouseButtonDown(GLFW_MOUSE_BUTTON_1))
		{
			float speed = Input::IsDown(GLFW_KEY_LEFT_SHIFT) ? 1.0f : 5.0f;
			myAngleAroundPlayer -= Input::MousePosDelta().x / speed;
		}
	}
	
	Player& myPlayer;
	float myDistanceFromPlayer = 50.0f;
	float myAngleAroundPlayer = 0.0f;

	float myNearZoom = 20.0f;
	float myFarZoom = 2000.0f;

	vec3 myFlyCamPosition;
public:
	vec3 myPosition;
	vec3 myRotation;
};