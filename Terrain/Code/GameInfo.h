#pragma once

// TODO:CW make this better
#include <map>
#include <string> 

#include "GL.h"

class EntityManager;
struct GLFWwindow;

class GameInfo
{
public:

	struct HeightInfo
	{
		float terrainHeight = 50.0f;
		float octives = 4;
		float amplitude = 15;
		float frequency = 1;
		float theScale = 0.005f;
		float persistance = 3.0f;
		float lacunarity = 0.2f;
	};
	static HeightInfo ourHeightInfo;

	static float ourDeltaTime;
	static float ourDayNightTime;
	static int ourScreenWidth;
	static int ourScreenHeight;
	static float ourWaterHeight;
	static float ourDayNightSpeed;
	static GLFWwindow* ourWindow;
	static bool ourWireframeMode;
	static bool ourDrawEntities;
	static bool ourDrawTerrain;
	static bool ourDrawWater;
	static bool ourDrawSkybox;
	static bool ourFlyCamera;
	static bool ourGenerateTerrainThreaded;
	static bool ourGenerateTerrainCaching;

	static void SetWindow(GLFWwindow* aWindow);
	static vec3 ourPlayerSpawnPos;

	static std::string ourCurrentRenderPass;
	static std::string ourCurrentShaderPass;
	static std::map<std::string, std::pair<double, unsigned int>> ourRenderTimes;

	static std::string GetPass();

	static void SetRenderTime(double aTime);

	static void SetRenderTriangles(unsigned int aVertCount);
};

#define Error(x) { printf(x); printf("\n"); system("color 47"); }
#define ErrorReturn(x) { printf(x); printf("\n"); system("color 47"); return -1; }