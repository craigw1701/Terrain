#pragma once

#include <map>

#include "Entity.h"
#include "MasterRenderer.h"
#include "Maths.h"
#include "RawModel.h"
#include "StaticShader.h"
#include "TexturedModel.h"

#include "glm/gtx/transform.hpp"

using namespace std;

static float locToRadians(float aDegree)
{
	return (aDegree * 3.14f) / 180.0f;
}

class EntityRenderer
{
public:
	EntityRenderer(StaticShader& aShader)
		:myShader(aShader)
	{
	}

	void Setup(glm::mat4 aProjectionMatrix)
	{
		myShader.Start();
		myShader.LoadProjectionMatrix(aProjectionMatrix);
		myShader.Stop();
	}

	void Render(map<TexturedModel*, vector<Entity*>>& someEntities)
	{
		for (auto& iter = someEntities.begin(); iter != someEntities.end(); iter++)
		{
			PrepareTexturedModel(*iter->first);
			for (Entity const* entity : iter->second)
			{
				PrepareInstance(*entity);
				GameInfo::SetRenderTriangles(iter->first->GetRawModel().GetVertexCount()/3);
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

		if (texture.myHasTransparency)
			DisableCulling();

		myShader.LoadFakeLighting(texture.myUseFakeLighting);
		myShader.LoadShineVariables(texture.myShineDamper, texture.myReflectivity);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());
	}

	void UnbindTexturedModel()
	{
		EnableCulling();
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
	StaticShader& myShader;

};