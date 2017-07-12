#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

#include "GameInfo.h"
#include "NonCopyable.h"

static int locThisFrameKeys[GLFW_KEY_LAST + 1] = { 0 };
static int locLastFrameKeys[GLFW_KEY_LAST + 1] = { 0 };
static vec2 locThisFrameScrollDelta;
static vec2 locLastFrameScrollDelta;
static int locThisFrameMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = {0};
static vec2 locThisFrameCursorPos;
static vec2 locLastFrameCursorPos;

class Input : public NonCopyable
{
public:
	static void Setup()
	{
		glfwSetScrollCallback(GameInfo::ourWindow, &Input::ScrollCallback);
	}

	static void UpdateInput()
	{
		for (int i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST+1; i++)
		{
			locLastFrameKeys[i] = locThisFrameKeys[i];
			locThisFrameKeys[i] = glfwGetKey(GameInfo::ourWindow, i);
		}
		for (int i = GLFW_MOUSE_BUTTON_1; i < GLFW_MOUSE_BUTTON_LAST+1; i++)
		{
			// TODO:CW LAST BUTTONS?
			locThisFrameMouseButtons[i] = glfwGetMouseButton(GameInfo::ourWindow, i);
		}
		locLastFrameCursorPos = locThisFrameCursorPos;
		double x, y;
		glfwGetCursorPos(GameInfo::ourWindow, &x, &y);
		locThisFrameCursorPos.x = static_cast<float>(x);
		locThisFrameCursorPos.y = static_cast<float>(y);

		locLastFrameScrollDelta = locThisFrameScrollDelta;
		locThisFrameScrollDelta = vec2(0.0f);
	}

	static bool IsMouseButtonDown(int aKey)
	{
		return locThisFrameMouseButtons[aKey] == GLFW_PRESS || locThisFrameMouseButtons[aKey] == GLFW_REPEAT;
	}

	static vec2 MousePosDelta()
	{
		return locLastFrameCursorPos - locThisFrameCursorPos;
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

	static vec2 GetScrollDelta()
	{
		return locLastFrameScrollDelta;
	}

private:
	static void ScrollCallback(GLFWwindow* aWindow, double aXoffset, double aYoffset)
	{
		locThisFrameScrollDelta = vec2(aXoffset, aYoffset);
	}
};