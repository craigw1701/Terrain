#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

class Light
{
public:
	Light(vec3 aPos, vec3 aColour)
		: myPosition(aPos), myColour(aColour)
	{

	}
	vec3 myPosition;
	vec3 myColour;

};