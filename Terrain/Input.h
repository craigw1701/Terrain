#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

#include "GameInfo.h"

static int locThisFrameKeys[GLFW_KEY_LAST + 1] = { 0 };
static int locLastFrameKeys[GLFW_KEY_LAST + 1] = { 0 };

class Input
{
public:
	static void UpdateInput()
	{
		for (int i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; i++)
		{
			locLastFrameKeys[i] = locThisFrameKeys[i];
			locThisFrameKeys[i] = glfwGetKey(GameInfo::ourWindow, i);
		}
	}

	static bool IsPressed(int aKey)
	{
		return locThisFrameKeys[aKey] == GLFW_PRESS && locLastFrameKeys[aKey] == GLFW_RELEASE;
	}
	
	static bool IsDown(int aKey)
	{
		return locThisFrameKeys[aKey] == GLFW_PRESS || locThisFrameKeys[aKey] == GLFW_REPEAT;
	}

	static bool IsReleased(int aKey)
	{
		return locThisFrameKeys[aKey] == GLFW_RELEASE && locLastFrameKeys[aKey] == GLFW_PRESS;
	}
};