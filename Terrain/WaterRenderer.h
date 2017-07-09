#pragma once

#include "Loader.h"
#include "Maths.h"
#include "RawModel.h"
#include "WaterShader.h"
#include "WaterTile.h"

class WaterRenderer
{
public:
	RawModel myQuad;
	WaterShader& myShader;
	WaterFrameBuffer& myFBOs;

	WaterRenderer(Loader& aLoader, WaterShader& aShader, WaterFrameBuffer& fbos)
		: myShader(aShader)
		, myQuad(aLoader.LoadToVAO({ { -1, 1 },{ -1, -1 },{ 1, 1 },{ 1, -1 } }))
		, myFBOs(fbos)
	{
		myShader.Setup();
	}

	void Setup(glm::mat4 aProjectionMatrix)
	{
		myShader.Start();
		myShader.ConnectTextureUnits();
		myShader.LoadProjectionMatrix(aProjectionMatrix);
		myShader.Stop();
	}

	void Render(vector<WaterTile>& someWater, Camera& aCamera)
	{
		if (!GameInfo::ourDrawWater)
			return;

		DisableCulling();
		PrepareRender(aCamera);
		for (WaterTile& waterTile : someWater)
		{
			mat4 modelMatrix = CreateTransformMatrix(waterTile.myCenterPos, vec3(0, 0, 0), waterTile.TILE_SIZE);
			myShader.LoadModelMatrix(modelMatrix);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, myQuad.GetVertexCount());
		}
		Unbind();
		EnableCulling();
	}

	void PrepareRender(Camera& aCamera)
	{
		myShader.Start();
		myShader.LoadViewMatrix(aCamera);
		glBindVertexArray(myQuad.GetVAOID());
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myFBOs.myReflectionTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, myFBOs.myRefractionTexture);
	}

	void Unbind()
	{
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		myShader.Stop();
	}
};
