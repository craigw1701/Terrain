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
		myLocationDUDVMap = GetUniformLocation("dudvMap");
		myLocationMoveFactor = GetUniformLocation("moveFactor");
		myLocationCameraPos = GetUniformLocation("cameraPos");
		myLocationNormalMap = GetUniformLocation("normalMap");
		myLocationLightColour = GetUniformLocation("lightColour");
		myLocationLightPosition = GetUniformLocation("lightPos");
		myLocationDepthMap = GetUniformLocation("depthMap");
	}

	void LoadProjectionMatrix(mat4 aMatrix)
	{
		LoadMatrix(myLocationProjectionMatrix, aMatrix);
	}

	void LoadViewMatrix(Camera& aCamera)
	{
		mat4 viewMatrix = CreateViewMatrix(aCamera);
		LoadMatrix(myLocationViewMatrix, viewMatrix);
		LoadVector(myLocationCameraPos, aCamera.myPosition);
	}

	void LoadModelMatrix(mat4 aMatrix)
	{
		LoadMatrix(myLocationModelMatrix, aMatrix);
	}

	void ConnectTextureUnits()
	{
		LoadInt(myLocationReflectionTexture, 0);
		LoadInt(myLocationRefractionTexture, 1);
		LoadInt(myLocationDUDVMap, 2);
		LoadInt(myLocationNormalMap, 3);
		LoadInt(myLocationDepthMap, 4);
	}

	void LoadLight(Light& aSun)
	{
		LoadVector(myLocationLightColour, aSun.myColour);
		LoadVector(myLocationLightPosition, aSun.myPosition);
	}

	void LoadMoveFactor(float aMoveFactor)
	{
		LoadFloat(myLocationMoveFactor, aMoveFactor);
	}

private:
	GLuint myLocationModelMatrix;
	GLuint myLocationViewMatrix;
	GLuint myLocationProjectionMatrix;
	GLuint myLocationReflectionTexture;
	GLuint myLocationRefractionTexture;
	GLuint myLocationDUDVMap;
	GLuint myLocationMoveFactor;
	GLuint myLocationCameraPos;
	GLuint myLocationNormalMap;
	GLuint myLocationLightColour;
	GLuint myLocationLightPosition;
	GLuint myLocationDepthMap;
};