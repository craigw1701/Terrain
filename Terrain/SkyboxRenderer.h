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
		
		myTextureID = aLoader.LoadCubeMap({ "right.png", "left.png", "top.png", "bottom.png", "back.png", "front.png" });
		myShader.Setup();
		myShader.Start();
		myShader.LoadProjectionMatrix(aProjectionMatrix);
		myShader.Stop();
	}

	void Render(Camera const& aCamera)
	{
		myShader.Start();
		myShader.LoadViewMatrix(aCamera);
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(myCube.GetVAOID());
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, myTextureID);
		glDrawArrays(GL_TRIANGLES, 0, myCube.GetVertexCount());
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
		myShader.Stop();
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
	SkyboxShader myShader;
};