#include "Input.h"

#include "GL.h"

#include "GameInfo.h"
#include "NonCopyable.h"
#include "DebugConsole.h"

int Input::locThisFrameKeys[GLFW_KEY_LAST + 1] = { 0 };
int Input::locLastFrameKeys[GLFW_KEY_LAST + 1] = { 0 };
vec2 Input::locThisFrameScrollDelta;
vec2 Input::locLastFrameScrollDelta;
int Input::locThisFrameMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };
vec2 Input::locThisFrameCursorPos;
vec2 Input::locLastFrameCursorPos;
vec2 Input::locDeltaCursorPos;


void Input::Setup()
{
	glfwSetScrollCallback(GameInfo::ourWindow, &Input::ScrollCallback);
	glfwSetWindowFocusCallback(GameInfo::ourWindow, &Input::WindowFocusCallback);
	double halfX = GameInfo::ourScreenWidth / 2.0f;
	double halfY = GameInfo::ourScreenHeight / 2.0f;
	glfwSetCursorPos(GameInfo::ourWindow, halfX, halfY);
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
	if (!GameInfo::ourWindowIsFocused)
		return;

	double halfX = GameInfo::ourScreenWidth / 2.0f;
	double halfY = GameInfo::ourScreenHeight / 2.0f;

	double x, y;
	glfwGetCursorPos(GameInfo::ourWindow, &x, &y);
	if (DebugConsole::IsActive())
	{
		locDeltaCursorPos = locLastFrameCursorPos - vec2(x, y);
	}
	else
	{
		locDeltaCursorPos = vec2(halfX, halfY) - vec2(x, y);
		glfwSetCursorPos(GameInfo::ourWindow, halfX, halfY);
	}
	locLastFrameCursorPos = vec2(x, y);

	locLastFrameScrollDelta = locThisFrameScrollDelta;
	locThisFrameScrollDelta = vec2(0.0f);

}

bool Input::IsMouseButtonDown(int aKey)
{
	return locThisFrameMouseButtons[aKey] == GLFW_PRESS || locThisFrameMouseButtons[aKey] == GLFW_REPEAT;
}

vec2 Input::MousePosDelta()
{
	return locDeltaCursorPos;
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

void Input::WindowFocusCallback(GLFWwindow* aWindow, int aIsInFocus)
{
	GameInfo::ourWindowIsFocused = aIsInFocus == GLFW_TRUE;

	double halfX = GameInfo::ourScreenWidth / 2.0f;
	double halfY = GameInfo::ourScreenHeight / 2.0f;
	glfwSetCursorPos(GameInfo::ourWindow, halfX, halfY);
	locDeltaCursorPos = vec2(0, 0);
}

/*static void CharacterInputCallback(GLFWwindow* aWindow, unsigned int aChar)
{
//locThisFrameScrollDelta = ;
printf("%d\n", aChar);
}*/