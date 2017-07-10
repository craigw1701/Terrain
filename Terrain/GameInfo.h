#pragma once

// TODO:CW make this better
#include <map>
#include <string> 

class EntityManager;

namespace GameInfo
{
	float ourDeltaTime;
	int ourScreenWidth = 1024;
	int ourScreenHeight = 768;
	GLFWwindow* ourWindow = nullptr;
	bool ourWireframeMode = false;
	bool ourDrawEntities = true;
	bool ourDrawTerrain = true;
	bool ourDrawWater = true;
	bool ourDrawSkybox = true;
	bool ourFlyCamera = false;	

	EntityManager* ourEntityManager = nullptr;
	std::string ourCurrentRenderPass = "Main";
	std::string ourCurrentShaderPass = "";
	std::map<std::string, std::pair<double, unsigned int>> ourRenderTimes;

	std::string GetPass()
	{
		std::string pass = GameInfo::ourCurrentRenderPass;
		if (ourCurrentShaderPass.size())
			pass += "/" + ourCurrentShaderPass;
		return pass;
	}

	void SetRenderTime(double aTime)
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

	void SetRenderTriangles(unsigned int aVertCount)
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

#define ErrorReturn(x) { printf(x); printf("\n"); system("color 47"); return -1; }