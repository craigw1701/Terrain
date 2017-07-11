#pragma once

// TODO:CW FIX HACK:
void EnableCulling()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void DisableCulling()
{
	glDisable(GL_CULL_FACE);
}


#include "Entity.h"
#include "Renderer.h"
#include "SkyboxRenderer.h"
#include "StaticShader.h"
#include "TerrainRenderer.h"
#include "TerrainShader.h"
#include "TexturedModel.h"

#include <map>
using namespace std;

class MasterRenderer
{
public:
	MasterRenderer(Loader& aLoader)
		: myProjectionMatrix(glm::perspectiveFov(myFOV, (float)GameInfo::ourScreenWidth, (float)GameInfo::ourScreenHeight, myNearPlane, myFarPlane))
		, myEntityShader()
		, myTerrainShader()
		, myEntityRenderer(myEntityShader)
		, myTerrainRenderer(myTerrainShader)
		, mySkyboxRenderer(aLoader, myProjectionMatrix)
	{
		EnableCulling();

		myEntityShader.Setup();
		myEntityRenderer.Setup(myProjectionMatrix);

		myTerrainShader.Setup();
		myTerrainRenderer.Setup(myProjectionMatrix);
	}

	~MasterRenderer()
	{
	}

	mat4 GetProjectionMatrix() const 
	{ 
		return myProjectionMatrix; 
	}

	void RenderScene(vector<Entity> const& someEntities, vector<Terrain> const& someTerrain, Light const& aSun, Camera const& aCamera, vec4 aClipPlane)
	{
		double startTime = glfwGetTime();

		for (Terrain const& terrain : someTerrain)
			ProcessTerrain(terrain);

		for (Entity const& entity : someEntities)
			ProcessEntity(entity);

		Render(aSun, aCamera, aClipPlane);

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
	void Render(Light const& aSun, Camera const& aCamera, vec4 aClipPlane) 
	{
		Prepare();
		
		if (GameInfo::ourDrawSkybox)
		{
			mySkyboxRenderer.Render(aCamera, aSun);
		}
		
		if (GameInfo::ourDrawEntities)
		{
			double startTime = glfwGetTime();
			myEntityShader.Start();
			myEntityShader.LoadClipPlane(aClipPlane);
			myEntityShader.LoadLight(aSun);
			myEntityShader.LoadViewMatrix(aCamera);
			myEntityRenderer.Render(myEntities);
			GameInfo::SetRenderTime(glfwGetTime() - startTime);
			myEntityShader.Stop();
		}

		if (GameInfo::ourDrawTerrain)
		{
			double startTime = glfwGetTime();
			myTerrainShader.Start();
			myTerrainShader.LoadClipPlane(aClipPlane);
			myTerrainShader.LoadLight(aSun);
			myTerrainShader.LoadViewMatrix(aCamera);
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

	void ProcessTerrain(Terrain const& aTerrain)
	{
		myTerrains.push_back(&aTerrain);
	}

private:
	float myFOV = 70;
	float myNearPlane = 0.1f;
	float myFarPlane = 5000.0f;
	glm::mat4 myProjectionMatrix;

	StaticShader myEntityShader;
	EntityRenderer myEntityRenderer;
	SkyboxRenderer mySkyboxRenderer;

	TerrainShader myTerrainShader;
	TerrainRenderer myTerrainRenderer;
	map<TexturedModel const*, vector<Entity const*>> myEntities; //TODO:CW don't really like these pointers...

	vector<Terrain const*> myTerrains; //TODO:CW don't really like these pointers...

	
};