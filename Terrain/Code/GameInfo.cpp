#include "GameInfo.h"

#include <map>
#include <string> 

#include "gl.h"

GameInfo::HeightInfo GameInfo::ourHeightInfo;
vec3 GameInfo::ourFogColour = vec3(0.5f, 0.5f, 0.5f);
float GameInfo::ourDeltaTime = 0.0f;
float GameInfo::ourDayNightTime = 0.0f;
int GameInfo::ourScreenWidth = 1024;
int GameInfo::ourScreenHeight = 768;
float GameInfo::ourWaterHeight = -20;
float GameInfo::ourDayNightSpeed = 0.0f;
GLFWwindow* GameInfo::ourWindow = nullptr;
bool GameInfo::ourWindowIsFocused = true;
bool GameInfo::ourIsShowingCursor = true;
bool GameInfo::ourWireframeMode = false;
bool GameInfo::ourDrawEntities = true;
bool GameInfo::ourDrawTerrain = true;
bool GameInfo::ourDrawWater = true;
bool GameInfo::ourDrawSkybox = true;
bool GameInfo::ourFlyCamera = true;
bool GameInfo::ourGenerateTerrainThreaded = true;

#ifdef _DEBUG
bool GameInfo::ourGenerateTerrainCaching = false;
#else
bool GameInfo::ourGenerateTerrainCaching = true;
#endif

void GameInfo::SetWindow(GLFWwindow* aWindow)
{
	ourWindow = aWindow;
}

vec3 GameInfo::ourPlayerSpawnPos = vec3(-100, 0, -100);

std::string GameInfo::ourCurrentRenderPass = "Main";
std::string GameInfo::ourCurrentShaderPass = "";
std::map<std::string, std::pair<double, unsigned int>> GameInfo::ourRenderTimes;

std::string GameInfo::GetPass()
{
	std::string pass = GameInfo::ourCurrentRenderPass;
	if (ourCurrentShaderPass.size())
		pass += "/" + ourCurrentShaderPass;
	return pass;
}

void GameInfo::SetRenderTime(double aTime)
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

void GameInfo::SetRenderTriangles(unsigned int aVertCount)
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