#pragma once
#include "ShaderBase.h"
#include "Maths.h"
#include "Light.h"

class TerrainShader : public ShaderBase
{
public:
	TerrainShader()
		: ShaderBase("terrain")
	{

	}

protected:
	void BindAttributes() override
	{
		BindAttribute(0, "position");
		BindAttribute(1, "textureCoords");
		BindAttribute(2, "normal");
	}

	virtual void GetAllUniformLocations() override
	{
		myLocationTransofmationMatrix = GetUniformLocation("transformationMatrix");
		myLocationProjectionMatrix = GetUniformLocation("projectionMatrix");
		myLocationViewMatrix = GetUniformLocation("viewMatrix");
		myLocationLightPosition = GetUniformLocation("lightPos");
		myLocationLightColour = GetUniformLocation("lightColour");
		myLocationShineDamper = GetUniformLocation("shineDamper");
		myLocationReflectivity = GetUniformLocation("reflectivity");
		myLocationBackgroundTexture = GetUniformLocation("backgroundTexture");
		myLocationRTexture = GetUniformLocation("rTexture");
		myLocationGTexture = GetUniformLocation("gTexture");
		myLocationBTexture = GetUniformLocation("bTexture");
		myLocationBlendTexture = GetUniformLocation("blendMap");
		myLocationPlane = GetUniformLocation("plane");
		myLocationSkyColour = GetUniformLocation("skyColour");
	}

public:
	void LoadTransformationMatrix(glm::mat4 aMatrix)
	{
		LoadMatrix(myLocationTransofmationMatrix, aMatrix);
	}
	void LoadProjectionMatrix(glm::mat4 aMatrix)
	{
		LoadMatrix(myLocationProjectionMatrix, aMatrix);
	}
	void LoadViewMatrix(Camera const& aCamera)
	{
		LoadMatrix(myLocationViewMatrix, CreateViewMatrix(aCamera));
	}

	void ConnectTextureUnits()
	{
		LoadInt(myLocationBackgroundTexture, 0);
		LoadInt(myLocationRTexture, 1);
		LoadInt(myLocationGTexture, 2);
		LoadInt(myLocationBTexture, 3);
		LoadInt(myLocationBlendTexture, 4);
	}

	void LoadLight(Light const& aLight)
	{
		LoadVector(myLocationLightPosition, aLight.myPosition);
		LoadVector(myLocationLightColour, aLight.myColour);
	}

	void LoadSkyColour(vec3 aColour)
	{
		LoadVector(myLocationSkyColour, aColour);
	}

	void LoadShineVariables(float aDamper, float aReflectivity)
	{
		LoadFloat(myLocationShineDamper, aDamper);
		LoadFloat(myLocationReflectivity, aReflectivity);
	}

	void LoadClipPlane(vec4 aPlane)
	{
		LoadVector(myLocationPlane, aPlane);
	}

private:
	GLuint myLocationTransofmationMatrix;
	GLuint myLocationProjectionMatrix;
	GLuint myLocationViewMatrix;
	GLuint myLocationLightPosition;
	GLuint myLocationLightColour;
	GLuint myLocationShineDamper;
	GLuint myLocationReflectivity;

	GLuint myLocationBackgroundTexture;
	GLuint myLocationRTexture;
	GLuint myLocationGTexture;
	GLuint myLocationBTexture;
	GLuint myLocationBlendTexture;
	GLuint myLocationPlane;

	GLuint myLocationSkyColour;
};