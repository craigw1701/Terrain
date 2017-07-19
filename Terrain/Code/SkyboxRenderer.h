#pragma once

#include "Loader.h"
#include "Maths.h"
#include "RawModel.h"
#include "SkyboxShader.h"

#define SKYBOX_SIZE 500.0f
class SkyboxRenderer
{
public:
	SkyboxRenderer(Loader& aLoader, mat4 aProjectionMatrix)
		: myCube(aLoader.LoadToVAO(myVertices))
	{
		
		myTextureID = aLoader.LoadCubeMap({ "right.png", "left.png", "top.png", "bottom.png", "back.png", "front.png" }, GL_TEXTURE0);
		myNightTextureID = aLoader.LoadCubeMap({ "SkyNight/NightRight.png", "SkyNight/Nightleft.png", "SkyNight/Nighttop.png", "SkyNight/Nightbottom.png", "SkyNight/Nightback.png", "SkyNight/Nightfront.png" }, GL_TEXTURE1);

		myShader.Setup();
		myShader.Start();
		myShader.ConnectTextureUnits();
		myShader.LoadProjectionMatrix(aProjectionMatrix);
		myShader.Stop();
	}

	void Render(Camera const& aCamera, Light const& aSun, vec3 aFogColour)
	{
		myShader.Start();
		myShader.LoadLight(aSun);
		myShader.LoadViewMatrix(aCamera);
		myShader.LoadFogColour(aFogColour);
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(myCube.GetVAOID());
		glEnableVertexAttribArray(0);
		BindTexture();
		GameInfo::SetRenderTriangles(myCube.GetVertexCount()/3);
		glDrawArrays(GL_TRIANGLES, 0, myCube.GetVertexCount());
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
		myShader.Stop();
	}

private:
	void BindTexture()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, myTextureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, myNightTextureID);
	}
	vector<vec3> myVertices = {
		{-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE},
		{-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE},
		{SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE},
		{SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE},
		{SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE},
		{-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE},

		{-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE},
		{-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE},
		{-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE},
		{-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE},
		{-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE},
		{-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE},

		{SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE},
		{SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE},
		{SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE},
		{SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE},
		{SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE},
		{SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE},

		{-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE},
		{-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE},
		{SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE},
		{SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE},
		{SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE},
		{-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE},

		{-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE},
		{SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE},
		{SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE},
		{SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE},
		{-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE},
		{-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE},

		{-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE},
		{-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE},
		{SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE},
		{SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE},
		{-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE},
		{SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE}
		};

	RawModel myCube;
	GLuint myTextureID;
	GLuint myNightTextureID;
	SkyboxShader myShader;
};