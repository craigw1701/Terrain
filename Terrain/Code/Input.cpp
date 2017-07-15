#include "Input.h"

#include "GL.h"

#include "GameInfo.h"
#include "NonCopyable.h"

int Input::locThisFrameKeys[GLFW_KEY_LAST + 1] = { 0 };
int Input::locLastFrameKeys[GLFW_KEY_LAST + 1] = { 0 };
vec2 Input::locThisFrameScrollDelta;
vec2 Input::locLastFrameScrollDelta;
int Input::locThisFrameMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };
vec2 Input::locThisFrameCursorPos;
vec2 Input::locLastFrameCursorPos;


void Input::Setup()
{
	glfwSetScrollCallback(GameInfo::ourWindow, &Input::ScrollCallback);
}

void Input::UpdateInput()
{
	for (int i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST + 1; i++)
	{
		locLastFrameKeys[i] = locThisFrameKeys[i];
		locThisFrameKeys[i] = glfwGetKey(GameInfo::ourWindow, i);
	}
	for (int i = GLFW_MOUSE_BUTTON_1; i < GLFW_MOUSE_BUTTON_LAST + 1; i++)
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

bool Input::IsMouseButtonDown(int aKey)
{
	return locThisFrameMouseButtons[aKey] == GLFW_PRESS || locThisFrameMouseButtons[aKey] == GLFW_REPEAT;
}

vec2 Input::MousePosDelta()
{
	return locLastFrameCursorPos - locThisFrameCursorPos;
}

bool Input::IsPressed(int aKey)
{
	return locThisFrameKeys[aKey] == GLFW_PRESS && locLastFrameKeys[aKey] == GLFW_RELEASE;
}

bool Input::IsDown(int aKey)
{
	return locThisFrameKeys[aKey] == GLFW_PRESS || locThisFrameKeys[aKey] == GLFW_REPEAT;
}

bool Input::IsReleased(int aKey)
{
	return locThisFrameKeys[aKey] == GLFW_RELEASE && locLastFrameKeys[aKey] == GLFW_PRESS;
}

vec2 Input::GetScrollDelta()
{
	return locLastFrameScrollDelta;
}

void Input::ScrollCallback(GLFWwindow* aWindow, double aXoffset, double aYoffset)
{
	locThisFrameScrollDelta = vec2(aXoffset, aYoffset);
}

/*static void CharacterInputCallback(GLFWwindow* aWindow, unsigned int aChar)
{
//locThisFrameScrollDelta = ;
printf("%d\n", aChar);
}*/