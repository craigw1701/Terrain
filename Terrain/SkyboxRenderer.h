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
		myNightTextureID = aLoader.LoadCubeMap({ "SkyNight/SkyNight_Right.png", "SkyNight/SkyNight_left.png", "SkyNight/SkyNight_top.png", "SkyNight/SkyNight_bottom.png", "SkyNight/SkyNight_back.png", "SkyNight/SkyNight_front.png" }, GL_TEXTURE1);

		myShader.Setup();
		myShader.Start();
		glUniform1i(glGetUniformLocation(myShader.myProgramID, "cubeMap"), 0);
		glUniform1i(glGetUniformLocation(myShader.myProgramID, "cubeMapNight"), 1);
		myShader.LoadProjectionMatrix(aProjectionMatrix);
		myShader.Stop();
	}

	void Render(Camera const& aCamera, Light const& aSun)
	{
		myShader.Start();
		myShader.LoadLight(aSun);
		myShader.LoadViewMatrix(aCamera);
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(myCube.GetVAOID());
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, myTextureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, myNightTextureID);
		GameInfo::SetRenderTriangles(myCube.GetVertexCount()/3);
		glDrawArrays(GL_TRIANGLES, 0, myCube.GetVertexCount());
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
		myShader.Stop();
	}

private:
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