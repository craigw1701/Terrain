#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

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