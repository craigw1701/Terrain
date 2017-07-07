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
	matrix = glm::rotate(matrix, aRotation.x, glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, aRotation.y, glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, aRotation.z, glm::vec3(0, 0, 1));
	matrix = glm::scale(matrix, glm::vec3(aScale, aScale, aScale));
	
	return matrix;
}


glm::mat4 CreateViewMatrix(Camera const& aCamera)
{
	glm::mat4 matrix(1.0f);

	matrix = glm::rotate(matrix, aCamera.myRotation.x, glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, aCamera.myRotation.y, glm::vec3(0, 1, 0));
	//matrix = glm::rotate(matrix, aCamera.myRotation.z, glm::vec3(0, 0, 1));

	vec3 negativeCameraPos = -aCamera.myPosition;
	matrix = glm::translate(matrix, negativeCameraPos);
	return matrix;
}

