#pragma once

#include "ShaderBase.h"
#include "Camera.h"
#include "Maths.h"

class SkyboxShader : public ShaderBase
{
public:
	SkyboxShader()
		: ShaderBase("skybox")
	{
	}

	void LoadProjectionMatrix(mat4 aMatrix) {
		LoadMatrix(myLocationProjectionMatrix, aMatrix);
	}

	void LoadViewMatrix(Camera const& aCamera) {
		mat4 matrix = CreateViewMatrix(aCamera);
		matrix[3][0] = 0.0f;
		matrix[3][1] = 0.0f;
		matrix[3][2] = 0.0f;
		LoadMatrix(myLocationViewMatrix, matrix);
	}

	void GetAllUniformLocations() override
	{
		myLocationProjectionMatrix = GetUniformLocation("projectionMatrix");
		myLocationViewMatrix = GetUniformLocation("viewMatrix");
	}

	void BindAttributes() override
	{
		BindAttribute(0, "position");
	}

private:
	GLuint myLocationProjectionMatrix;
	GLuint myLocationViewMatrix;

};