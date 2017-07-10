#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "Camera.h"
using namespace glm;


glm::mat4 CreateTransformMatrix(glm::vec3 aTranslation, glm::vec3 aRotation, float aScale)
{
	glm::mat4 matrix(1.0f);
	matrix = glm::translate(matrix, aTranslation);
	matrix = glm::rotate(matrix, radians(aRotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, radians(aRotation.y), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, radians(aRotation.z), glm::vec3(0, 0, 1));
	matrix = glm::scale(matrix, glm::vec3(aScale, aScale, aScale));
	
	return matrix;
}

glm::mat4 CreateTransformationMatrix(vec2 aTranslation, vec2 aScale) 
{
	glm::mat4 matrix(1.0f);
	matrix = glm::translate(matrix, vec3(aTranslation, 0));
	matrix = glm::scale(matrix, glm::vec3(aScale.x, aScale.y, 1.0f));
	return matrix;
}

glm::mat4 CreateViewMatrix(Camera const& aCamera)
{
	glm::mat4 matrix(1.0f);

	matrix = glm::rotate(matrix, radians(aCamera.myRotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, radians(aCamera.myRotation.y), glm::vec3(0, 1, 0));
	//matrix = glm::rotate(matrix, aCamera.myRotation.z, glm::vec3(0, 0, 1));

	vec3 negativeCameraPos = -aCamera.myPosition;
	matrix = glm::translate(matrix, negativeCameraPos);
	return matrix;
}

float RandFloat(float aMin, float aMax)
{
	return aMin + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (aMax - aMin));
}

vec2 RandVector(vec2 aMin, vec2 aMax)
{
	vec2 vec;

	vec.x = RandFloat(aMin.x, aMax.x);
	vec.y = RandFloat(aMin.y, aMax.y);

	return vec;
}

vec3 RandVector(vec3 aMin, vec3 aMax)
{
	vec3 vec;

	vec.x = RandFloat(aMin.x, aMax.x);
	vec.y = RandFloat(aMin.y, aMax.y);
	vec.z = RandFloat(aMin.z, aMax.z);

	return vec;
}

