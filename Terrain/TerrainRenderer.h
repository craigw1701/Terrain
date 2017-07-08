#pragma once

#include "TerrainShader.h"
#include "TexturedModel.h"
#include "RawModel.h"
#include "Terrain.h"

class TerrainRenderer
{
public:
	TerrainRenderer(TerrainShader& aShader)
		: myShader(aShader)
	{
	}
	
	void Setup(glm::mat4 aProjectionMatrix)
	{
		myShader.Start();
		myShader.LoadProjectionMatrix(aProjectionMatrix);
		myShader.ConnectTextureUnits();
		myShader.Stop();
	}

	void Render(vector<Terrain*> someTerrains)
	{
		for (Terrain* terrain : someTerrains)
		{
			PrepareTerrain(*terrain);
			LoadModelMatrix(*terrain);
			glDrawElements(GL_TRIANGLES, terrain->GetModel().GetVertexCount(), GL_UNSIGNED_INT, nullptr);
			UnbindTexturedModel();
		}
	}


private:
	void PrepareTerrain(Terrain const& aTerrain)
	{
		RawModel const& model = aTerrain.GetModel();
		glBindVertexArray(model.GetVAOID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		BindTextures(aTerrain);
		myShader.LoadShineVariables(1, 0);
	}

	void BindTextures(Terrain const& aTerrain)
	{
		TerrainTexturePack const& texturePack = aTerrain.GetTextures();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePack.myBackgroundTexture.myTextureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texturePack.myRTexture.myTextureID);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texturePack.myGTexture.myTextureID);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texturePack.myBTexture.myTextureID);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, aTerrain.GetBlendMap().myTextureID);
	}

	void UnbindTexturedModel()
	{
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(3);
		glBindVertexArray(0);
	}

	void LoadModelMatrix(Terrain const& aTerrain)
	{
		glm::mat4 transformationMatrix = CreateTransformMatrix(vec3(aTerrain.GetX(), 0, aTerrain.GetZ()), vec3(0, 0, 0), 1.0f);
		myShader.LoadTransformationMatrix(transformationMatrix);
	}

	TerrainShader& myShader;


};