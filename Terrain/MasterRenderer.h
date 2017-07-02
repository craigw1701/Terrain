#pragma once

#include "StaticShader.h"
#include "Renderer.h"
#include "TexturedModel.h"
#include "Entity.h"

#include <map>
using namespace std;

class MasterRenderer
{
public:
	MasterRenderer()
		:myShader(), myRenderer(myShader)
	{
		myShader.Setup();
		myRenderer.Setup();
	}

	~MasterRenderer()
	{
	}

	void Render(Light& aSun, Camera& aCamera)
	{
		myRenderer.Prepare();
		myShader.Start();
		myShader.LoadLight(aSun);
		myShader.LoadViewMatrix(aCamera);

		myRenderer.Render(myEntities);

		myShader.Stop();
		myEntities.clear();
	}

	void ProcessEntity(Entity& anEntity)
	{
		TexturedModel& entityModel = anEntity.GetModel();

		auto& iter = myEntities.find(&entityModel);
		if (iter != myEntities.end())
		{
			iter->second.push_back(&anEntity);
		}
		else
		{
			vector<Entity*> list;
			list.push_back(&anEntity);
			myEntities[&entityModel] = list;
		}
	}

private:
	StaticShader myShader;
	Renderer myRenderer;
	map<TexturedModel*, vector<Entity*>> myEntities; //TODO:CW don't really like these pointers...
	
};