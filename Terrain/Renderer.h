#pragma once

#include <map>

#include "RawModel.h"
#include "TexturedModel.h"
#include "Entity.h"
#include "StaticShader.h"
#include "Maths.h"

#include "glm/gtx/transform.hpp"

using namespace std;

static float locToRadians(float aDegree)
{
	return (aDegree * 3.14f) / 180.0f;
}

class Renderer
{
public:
	Renderer(StaticShader& aShader)
		:myShader(aShader)
	{
	}

	void Setup()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		myProjectionMatrix = glm::perspectiveFov(myFOV, (float)GameInfo::ourScreenWidth, (float)GameInfo::ourScreenHeight, myNearPlane, myFarPlane);
		myShader.Start();
		myShader.LoadProjectionMatrix(myProjectionMatrix);
		myShader.Stop();
	}
	
	void Prepare()
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);
	}

	void Render(map<TexturedModel*, vector<Entity*>>& someEntities)
	{
		for (auto& iter = someEntities.begin(); iter != someEntities.end(); iter++)
		{
			PrepareTexturedModel(*iter->first);
			for (Entity const* entity : iter->second)
			{
				PrepareInstance(*entity);
				glDrawElements(GL_TRIANGLES, iter->first->GetRawModel().GetVertexCount(), GL_UNSIGNED_INT, nullptr);

			}
			UnbindTexturedModel();
		}

	}

	void PrepareTexturedModel(TexturedModel const& aModel)
	{
		RawModel& model = aModel.GetRawModel();
		glBindVertexArray(model.GetVAOID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		ModelTexture const& texture = aModel.GetTexture();
		myShader.LoadShineVariables(texture.myShineDamper, texture.myReflectivity);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());
	}

	void UnbindTexturedModel()
	{
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(3);
		glBindVertexArray(0);
	}

	void PrepareInstance(Entity const& anEntity)
	{
		glm::mat4 transformationMatrix = CreateTransformMatrix(anEntity.myPosition, anEntity.myRotation, anEntity.myScale);
		myShader.LoadTransformationMatrix(transformationMatrix);

	}

private:
	float myFOV = 70;
	float myNearPlane = 0.1f;
	float myFarPlane = 1000.0f;
	glm::mat4 myProjectionMatrix;
	StaticShader& myShader;

};