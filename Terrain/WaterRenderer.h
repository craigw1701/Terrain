#pragma once

#include "Loader.h"
#include "Maths.h"
#include "RawModel.h"
#include "WaterShader.h"
#include "WaterTile.h"

#define WAVE_SPEED 0.03f

class WaterRenderer
{
public:
	RawModel myQuad;
	WaterShader& myShader;
	WaterFrameBuffer& myFBOs;
	GLuint myDUDVMap;
	GLuint myNormalMap;

	float myMoveFactor = 0;

	WaterRenderer(Loader& aLoader, WaterShader& aShader, WaterFrameBuffer& fbos)
		: myShader(aShader)
		, myQuad(aLoader.LoadToVAO({ { -1, 1 },{ -1, -1 },{ 1, 1 },{ 1, -1 } }))
		, myFBOs(fbos)
	{
		myDUDVMap = aLoader.LoadTexture("waterDUDV.png");
		myNormalMap = aLoader.LoadTexture("normal.png");
		myShader.Setup();
	}

	void Setup(glm::mat4 aProjectionMatrix)
	{
		myShader.Start();
		myShader.ConnectTextureUnits();
		myShader.LoadProjectionMatrix(aProjectionMatrix);
		myShader.Stop();
	}

	void Render(vector<WaterTile>& someWater, Camera& aCamera, Light& aLight)
	{
		if (!GameInfo::ourDrawWater)
			return;

		DisableCulling();
		PrepareRender(aCamera, aLight);
		for (WaterTile& waterTile : someWater)
		{
			mat4 modelMatrix = CreateTransformMatrix(waterTile.myCenterPos, vec3(0, 0, 0), waterTile.TILE_SIZE);
			myShader.LoadModelMatrix(modelMatrix);
			GameInfo::SetRenderVerts(myQuad.GetVertexCount());
			glDrawArrays(GL_TRIANGLE_STRIP, 0, myQuad.GetVertexCount());
		}
		Unbind();
		EnableCulling();
	}

	void PrepareRender(Camera& aCamera, Light& aLight)
	{
		myShader.Start();
		myShader.LoadViewMatrix(aCamera);
		myMoveFactor += WAVE_SPEED * GameInfo::ourDeltaTime;
		myMoveFactor = myMoveFactor - (int)myMoveFactor;
		myShader.LoadMoveFactor(myMoveFactor);
		myShader.LoadLight(aLight);
		glBindVertexArray(myQuad.GetVAOID());
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myFBOs.myReflectionTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, myFBOs.myRefractionTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, myDUDVMap);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, myNormalMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, myFBOs.myRefractionDepthTexture);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Unbind()
	{
		glDisable(GL_BLEND);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		myShader.Stop();
	}
};
