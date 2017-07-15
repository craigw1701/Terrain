#pragma once

// TODO:CW make this better
#include <map>
#include <string> 

class EntityManager;

namespace GameInfo
{
	static float ourDeltaTime = 0.0f;
	static float ourDayNightTime = 0.0f;
	static int ourScreenWidth = 1024;
	static int ourScreenHeight = 768;
	static float ourWaterHeight = -20;
	static float ourDayNightSpeed = 0.0f;
	static GLFWwindow* ourWindow = nullptr;
	static bool ourWireframeMode = false;
	static bool ourDrawEntities = true;
	static bool ourDrawTerrain = true;
	static bool ourDrawWater = true;
	static bool ourDrawSkybox = true;
	static bool ourFlyCamera = false;
	static bool ourGenerateTerrainThreaded = true;
#ifdef _DEBUG
	static bool ourGenerateTerrainCaching = false;
#else
	static bool ourGenerateTerrainCaching = true;
#endif

	static vec3 ourPlayerSpawnPos(-100, 0, -100);

	static std::string ourCurrentRenderPass = "Main";
	static std::string ourCurrentShaderPass = "";
	static std::map<std::string, std::pair<double, unsigned int>> ourRenderTimes;

	static std::string GetPass()
	{
		std::string pass = GameInfo::ourCurrentRenderPass;
		if (ourCurrentShaderPass.size())
			pass += "/" + ourCurrentShaderPass;
		return pass;
	}

	static void SetRenderTime(double aTime)
	{
		std::string pass = GetPass();
		auto iter = ourRenderTimes.find(pass);
		if (iter != ourRenderTimes.end())
		{
			iter->second.first += aTime;
			return;
		}

		ourRenderTimes[pass].first = aTime;
		ourRenderTimes[pass].second = 0;
	}

	static void SetRenderTriangles(unsigned int aVertCount)
	{
		std::string pass = GetPass();
		auto iter = ourRenderTimes.find(pass);
		if (iter != ourRenderTimes.end())
		{
			iter->second.second += aVertCount;
			return;
		}

		ourRenderTimes[pass].first = 0;
		ourRenderTimes[pass].second = aVertCount;
	}
};

#define Error(x) { printf(x); printf("\n"); system("color 47"); }
#define ErrorReturn(x) { printf(x); printf("\n"); system("color 47"); return -1; }