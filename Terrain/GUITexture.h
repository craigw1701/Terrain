#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"

using namespace glm;

class GUITexture
{
public:
	GUITexture(GLuint aTextureID, vec2 aPosition, vec2 aScale)
		: myTextureID(aTextureID)
		, myPosition(aPosition)
		, myScale(aScale)
	{}

	GLuint myTextureID;
	vec2 myPosition;
	vec2 myScale;
};
