#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL.h"

// Include GLM
#include "../glm/glm.hpp"
#include "../glm/gtx/transform.hpp"
using namespace glm;

static unsigned int g_seed;

// Used to seed the generator.           
inline void fast_srand(int seed) {
	g_seed = seed;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
inline int fast_rand(void) {
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}

static float BarryCentric(vec3 p1, vec3 p2, vec3 p3, vec2 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}


inline glm::mat4 CreateTransformMatrix(glm::vec3 aTranslation, glm::vec3 aRotation, float aScale)
{
	glm::mat4 matrix(1.0f);
	matrix = glm::translate(matrix, aTranslation);
	matrix = glm::rotate(matrix, radians(aRotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, radians(aRotation.y), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, radians(aRotation.z), glm::vec3(0, 0, 1));
	matrix = glm::scale(matrix, glm::vec3(aScale, aScale, aScale));

	return matrix;
}


#include "Camera.h" // TODO:CW FIX ALL THE HACKS

inline glm::mat4 CreateTransformationMatrix(vec2 aTranslation, vec2 aScale)
{
	glm::mat4 matrix(1.0f);
	matrix = glm::translate(matrix, vec3(aTranslation, 0));
	matrix = glm::scale(matrix, glm::vec3(aScale.x, aScale.y, 1.0f));
	return matrix;
}

inline glm::mat4 CreateViewMatrix(Camera const& aCamera)
{
	glm::mat4 matrix(1.0f);

	matrix = glm::rotate(matrix, radians(aCamera.myRotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, radians(aCamera.myRotation.y), glm::vec3(0, 1, 0));
	//matrix = glm::rotate(matrix, aCamera.myRotation.z, glm::vec3(0, 0, 1));

	vec3 negativeCameraPos = -aCamera.myPosition;
	matrix = glm::translate(matrix, negativeCameraPos);
	return matrix;
}

inline float RandFloat(float aMin, float aMax)
{
	return aMin + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (aMax - aMin));
}

inline vec2 RandVector(vec2 aMin, vec2 aMax)
{
	vec2 vec;

	vec.x = RandFloat(aMin.x, aMax.x);
	vec.y = RandFloat(aMin.y, aMax.y);

	return vec;
}

inline vec3 RandVector(vec3 aMin, vec3 aMax)
{
	vec3 vec;

	vec.x = RandFloat(aMin.x, aMax.x);
	vec.y = RandFloat(aMin.y, aMax.y);
	vec.z = RandFloat(aMin.z, aMax.z);

	return vec;
}

