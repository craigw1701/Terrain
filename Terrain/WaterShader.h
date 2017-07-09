#pragma once

#include "ShaderBase.h"

#include "Maths.h"

class WaterShader : public ShaderBase
{
public:
	WaterShader()
		: ShaderBase("watershader")
	{

	}

	void BindAttributes() override
	{
		BindAttribute(0, "position");
	}

	void GetAllUniformLocations() override
	{
		myLocationProjectionMatrix = GetUniformLocation("projectionMatrix");
		myLocationViewMatrix = GetUniformLocation("viewMatrix");
		myLocationModelMatrix = GetUniformLocation("modelMatrix");
		myLocationReflectionTexture = GetUniformLocation("reflectionTexture");
		myLocationRefractionTexture = GetUniformLocation("refractionTexture");
	}

	void LoadProjectionMatrix(mat4 aMatrix)
	{
		LoadMatrix(myLocationProjectionMatrix, aMatrix);
	}

	void LoadViewMatrix(Camera& aCamera)
	{
		mat4 viewMatrix = CreateViewMatrix(aCamera);
		LoadMatrix(myLocationViewMatrix, viewMatrix);
	}

	void LoadModelMatrix(mat4 aMatrix)
	{
		LoadMatrix(myLocationModelMatrix, aMatrix);
	}

	void ConnectTextureUnits()
	{
		LoadInt(myLocationReflectionTexture, 0);
		LoadInt(myLocationRefractionTexture, 1);
	}

	GLuint myLocationModelMatrix;
	GLuint myLocationViewMatrix;
	GLuint myLocationProjectionMatrix;
	GLuint myLocationReflectionTexture;
	GLuint myLocationRefractionTexture;
};