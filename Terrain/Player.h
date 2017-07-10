#pragma once

#include "Entity.h"
#include "Input.h"

class Player : public Entity
{
public:
	float myRunSpeed = 20.0f;
	float myTurnSpeed = 5.0f;
	float myGravity = -50.0f;
	float myJumpPower = 30;
	float myTerrainHeight = 0.0f;

	float myCurrentSpeed = 0.0f;
	float myCurrentTurnSpeed = 0.0f;
	float myUpwardsSpeed = 0;
	bool myIsInAir = false;

	Player(TexturedModel& aModel, glm::vec3 aPosition, glm::vec3 aRotation, float aScale)
		: Entity(aModel, aPosition, aRotation, aScale)
	{

	}

	void Update()
	{
		CheckInputs();
		myRotation += vec3(0, myCurrentTurnSpeed * GameInfo::ourDeltaTime, 0);
		float distance = myCurrentSpeed * GameInfo::ourDeltaTime;

		float dX = distance * sin(myRotation.y);
		float dZ = distance * cos(myRotation.y);

		myPosition += vec3(dX, 0, dZ);

		myUpwardsSpeed += myGravity * GameInfo::ourDeltaTime;
		myPosition.y += myUpwardsSpeed * GameInfo::ourDeltaTime;

		if (myPosition.y < myTerrainHeight)
		{
			myIsInAir = false;
			myUpwardsSpeed = 0;
			myPosition.y = myTerrainHeight;
		}
	}

private:
	void Jump()
	{
		if (myIsInAir)
			return;

		myUpwardsSpeed = myJumpPower;
		myIsInAir = true;
	}

	void CheckInputs()
	{
		if (Input::IsDown(GLFW_KEY_W))
		{
			myCurrentSpeed = myRunSpeed;
		}
		else if(Input::IsDown(GLFW_KEY_S))
		{
			myCurrentSpeed = -myRunSpeed;
		}
		else
		{
			myCurrentSpeed = 0;
		}

		if (Input::IsDown(GLFW_KEY_D))
		{
			myCurrentTurnSpeed = -myTurnSpeed;
		}
		else if (Input::IsDown(GLFW_KEY_A))
		{
			myCurrentTurnSpeed = myTurnSpeed;
		}
		else
		{
			myCurrentTurnSpeed = 0;
		}


		if (Input::IsDown(GLFW_KEY_SPACE))
		{
			Jump();
		}
	}

};