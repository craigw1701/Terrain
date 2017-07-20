#pragma once

#include "Sun.h"
#include "SunShader.h"
#include "Loader.h"
#include "Camera.h"
#include "Maths.h"

class SunRenderer
{
	vector<vec2> positions{ { -0.5f, 0.5f },{ -0.5f, -0.5f },{ 0.5f, 0.5f },{ 0.5f, -0.5f } };
	SunShader myShader;
	RawModel myQuad;

public:
	SunRenderer(Loader &aLoader)
		: myShader()
		, myQuad(aLoader.LoadToVAO(positions))
	{
		myShader.Setup();
		myShader.Start();
		myShader.ConnectTextureUnits();
		//myShader.LoadTransformationMatrix(aProjectionMatrix)
		myShader.Stop();
	}

	void Render(Sun const& aSun, Camera const& aCamera, glm::mat4 aProjectionMatrix)
	{
		DisableCulling();
		Antialias(true);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		myShader.Start();

		//mat4 matrix = CreateTransformationMatrix(vec2(0.5, 0.5), vec2(1,1));
		mat4 matrix = CalculateMvpMatrix(aSun, aCamera, aProjectionMatrix);
		myShader.LoadTransformationMatrix(matrix);
		myShader.LoadSunColour(aSun.GetColour());

		glBindVertexArray(myQuad.GetVAOID());
		glEnableVertexAttribArray(0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, aSun.GetTexture().GetTextureID());

		GameInfo::SetRenderTriangles(myQuad.GetVertexCount() - 2);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, myQuad.GetVertexCount());

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		myShader.Stop();
		Antialias(false);
	}

private:
	void Antialias(bool enable) 
	{
		if (enable)
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}
	}
	
	mat4 CalculateMvpMatrix(Sun const& sun, Camera const& camera, glm::mat4 aProjectionMatrix)
	{
		mat4 modelMatrix(1);
		vec3 sunPos = sun.GetWorldPosition(camera.myPosition);
		modelMatrix = glm::translate(modelMatrix, sunPos);

		mat4 modelViewMat = ApplyViewMatrix(modelMatrix, CreateViewMatrix(camera));
		modelViewMat = glm::scale(modelViewMat, glm::vec3(sun.GetScale()));
				
		return aProjectionMatrix * modelViewMat;
	}

	//TODO:CW watch particle tutorial
	mat4 ApplyViewMatrix(mat4 modelMatrix, mat4 viewMatrix) {
		modelMatrix[0][0] = viewMatrix[0][0];
		modelMatrix[0][1] = viewMatrix[1][0];
		modelMatrix[0][2] = viewMatrix[2][0];
		modelMatrix[1][0] = viewMatrix[0][1];
		modelMatrix[1][1] = viewMatrix[1][1];
		modelMatrix[1][2] = viewMatrix[2][1];
		modelMatrix[2][0] = viewMatrix[0][2];
		modelMatrix[2][1] = viewMatrix[1][2];
		modelMatrix[2][2] = viewMatrix[2][2];

		return viewMatrix * modelMatrix;
	}
};
