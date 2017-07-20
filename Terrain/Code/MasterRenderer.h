#pragma once

// TODO:CW FIX HACK:
static void EnableCulling()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

static void DisableCulling()
{
	glDisable(GL_CULL_FACE);
}


#include "Entity.h"
#include "NonCopyable.h"
#include "Renderer.h"
#include "SkyboxRenderer.h"
#include "StaticShader.h"
#include "TerrainRenderer.h"
#include "TerrainShader.h"
#include "TexturedModel.h"

#include <map>
#include "SunRenderer.h"
using namespace std;

// TODO:CW Write proper Debug Console Variables wrapper
/*#define ConsoleVariable(cmd, name) static bool DebugVariable_##name = false; DebugConsole::AddCommand(cmd, "[x] - 0 = off, 1 = on; empty = toggle", [](std::string aFullCommand){ DebugVariable_##name = !DebugVariable_##name; }) 

ConsoleVariable("Render.ShowEntities", Entities);*/

class MasterRenderer : public NonCopyable
{
public:
	MasterRenderer(Loader& aLoader, SunRenderer& aSunRenderer)
		: myProjectionMatrix(glm::perspectiveFov(myFOV, (float)GameInfo::ourScreenWidth, (float)GameInfo::ourScreenHeight, myNearPlane, myFarPlane))
		, myEntityShader()
		, myTerrainShader()
		, myEntityRenderer(myEntityShader)
		, myTerrainRenderer(myTerrainShader)
		, mySkyboxRenderer(aLoader, myProjectionMatrix)
		, mySunRenderer(aSunRenderer)
	{
		EnableCulling();

		myEntityShader.Setup();
		myEntityRenderer.Setup(myProjectionMatrix);

		myTerrainShader.Setup();
		myTerrainRenderer.Setup(myProjectionMatrix);

		if (DebugConsole* console = DebugConsole::GetInstance())
		{
			console->AddVariable("Render.Wireframe", GameInfo::ourWireframeMode);
			console->AddVariable("Render.ShowWater", GameInfo::ourDrawWater);
			console->AddVariable("Render.ShowEntities", GameInfo::ourDrawEntities);
			console->AddVariable("Render.ShowTerrain", GameInfo::ourDrawTerrain);
			console->AddVariable("Render.ShowSkybox", GameInfo::ourDrawSkybox);
			console->AddVariable("Render.FOV", myFOV);
			console->AddVariable("Render.NearPlane", myNearPlane);
			console->AddVariable("Render.FarPlane", myFarPlane);
			console->AddCommand("Render.UpdateProjection", "", [this](std::vector<std::string> someParams)
			{
				myProjectionMatrix = glm::perspectiveFov(myFOV, (float)GameInfo::ourScreenWidth, (float)GameInfo::ourScreenHeight, myNearPlane, myFarPlane);
				return "Updated Projection";
			});
		}
	}

	~MasterRenderer()
	{
	}

	mat4 GetProjectionMatrix() const 
	{ 
		return myProjectionMatrix; 
	}

	void RenderScene(vector<Entity> const& someEntities, Terrain::TerrainList const& someTerrain, Sun const& aSun, Camera const& aCamera, vec4 aClipPlane)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GameInfo::ourWireframeMode ? GL_LINE : GL_FILL);
		double startTime = glfwGetTime();

		for (auto const& terrain : someTerrain)
			ProcessTerrain(terrain.get());

		for (Entity const& entity : someEntities)
			ProcessEntity(entity);

		Render(aSun, aCamera, aClipPlane);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		GameInfo::SetRenderTime(glfwGetTime() - startTime);
	}

	void ProcessEntity(Entity const& anEntity)
	{
		TexturedModel& entityModel = anEntity.GetModel();

		auto& iter = myEntities.find(&entityModel);
		if (iter != myEntities.end())
		{
			iter->second.push_back(&anEntity);
		}
		else
		{
			vector<Entity const*> list;
			list.push_back(&anEntity);
			myEntities[&entityModel] = list;
		}
	}

private:
	void Render(Sun const& aSun, Camera const& aCamera, vec4 aClipPlane) 
	{
		Prepare();
		
		if (GameInfo::ourDrawSkybox)
		{
			mySkyboxRenderer.Render(aCamera, aSun, GameInfo::ourFogColour);
		}

		mySunRenderer.Render(aSun, aCamera, myProjectionMatrix);
		if (GameInfo::ourDrawEntities)
		{
			double startTime = glfwGetTime();
			myEntityShader.Start();
			myEntityShader.LoadClipPlane(aClipPlane);
			myEntityShader.LoadLight(aSun.GetLight());
			myEntityShader.LoadViewMatrix(aCamera);
			myEntityRenderer.Render(myEntities);
			GameInfo::SetRenderTime(glfwGetTime() - startTime);
			myEntityShader.Stop();
		}

		if (GameInfo::ourDrawTerrain)
		{
			EnableCulling();
			double startTime = glfwGetTime();
			myTerrainShader.Start();
			myTerrainShader.LoadClipPlane(aClipPlane);
			myTerrainShader.LoadLight(aSun.GetLight());
			myTerrainShader.LoadViewMatrix(aCamera);
			myTerrainShader.LoadSkyColour(GameInfo::ourFogColour);
			myTerrainRenderer.Render(myTerrains);
			GameInfo::SetRenderTime(glfwGetTime() - startTime);
			myTerrainShader.Stop();
		}
		
		myEntities.clear();
		myTerrains.clear();
	}

	void Prepare()
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);
	}

	void ProcessTerrain(Terrain const* aTerrain)
	{
		myTerrains.push_back(aTerrain);
	}

private:
	float myFOV = 70;
	float myNearPlane = 0.1f;
	float myFarPlane = 5000.0f;
	glm::mat4 myProjectionMatrix;

	StaticShader myEntityShader;
	EntityRenderer myEntityRenderer;
	SkyboxRenderer mySkyboxRenderer;
	SunRenderer& mySunRenderer;

	TerrainShader myTerrainShader;
	TerrainRenderer myTerrainRenderer;
	map<TexturedModel const*, vector<Entity const*>> myEntities; //TODO:CW don't really like these pointers...

	vector<Terrain const*> myTerrains; //TODO:CW don't really like these pointers...

	
};