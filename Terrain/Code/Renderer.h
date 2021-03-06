#pragma once

#include <map>

#include "Entity.h"
#include "MasterRenderer.h"
#include "Maths.h"
#include "RawModel.h"
#include "StaticShader.h"
#include "TexturedModel.h"
#include "gl.h"

using namespace std;

class EntityRenderer
{
public:
	EntityRenderer(StaticShader& aShader)
		:myShader(aShader)
	{
	}

	void Setup()
	{
		myShader.Start();
		myShader.LoadProjectionMatrix(GameInfo::ourProjectionMatrix);
		myShader.Stop();
	}

	void Render(map<TexturedModel const*, vector<Entity const*>>& someEntities)
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

private:
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