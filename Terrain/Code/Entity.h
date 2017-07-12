#pragma once

#include "GL.h"

#include "TexturedModel.h"

class Entity
{
public:
	Entity(TexturedModel& aModel, glm::vec3 aPosition, glm::vec3 aRotation, float aScale)
		: myModel(aModel)
		, myPosition(aPosition)
		, myRotation(aRotation)
		, myScale(aScale)
	{

	}

	TexturedModel& GetModel() const { return myModel; }

public:
	glm::vec3 myPosition;
	glm::vec3 myRotation;
	float myScale;


private:
	TexturedModel& myModel;
};