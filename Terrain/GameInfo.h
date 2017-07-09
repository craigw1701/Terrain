#pragma once

// TODO:CW make this better

namespace GameInfo
{
	float ourDeltaTime;
	int ourScreenWidth = 1024;
	int ourScreenHeight = 768;
	GLFWwindow* ourWindow = nullptr;
	bool ourWireframeMode = false;
	bool ourDrawEntities = false;
	bool ourDrawTerrain = true;
	bool ourDrawWater = true;
	bool ourDrawSkybox = true;
};