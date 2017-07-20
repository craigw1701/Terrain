#pragma once

#include "GL.h"

#include "NonCopyable.h"

class Input : public NonCopyable
{
public:
	static void Setup();

	static void UpdateInput();

	static bool IsMouseButtonDown(int aKey);

	static vec2 MousePosDelta();

	static bool IsPressed(int aKey);
	
	static bool IsDown(int aKey);

	static bool IsReleased(int aKey);

	static vec2 GetScrollDelta();

private:
	static void ScrollCallback(GLFWwindow* aWindow, double aXoffset, double aYoffset);
	static void WindowFocusCallback(GLFWwindow* aWindow, int aIsInFocus);

	/*static void CharacterInputCallback(GLFWwindow* aWindow, unsigned int aChar)
	{
		//locThisFrameScrollDelta = ;
		printf("%d\n", aChar);
	}*/
	static int locThisFrameKeys[GLFW_KEY_LAST + 1];
	static int locLastFrameKeys[GLFW_KEY_LAST + 1];
	static vec2 locThisFrameScrollDelta;
	static vec2 locLastFrameScrollDelta;
	static int locThisFrameMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
	static vec2 locThisFrameCursorPos;
	static vec2 locLastFrameCursorPos;
	static vec2 locDeltaCursorPos;
};