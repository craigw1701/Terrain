#pragma once
#include "ShaderBase.h"
#include "Maths.h"
#include "Light.h"

class StaticShader : public ShaderBase
{
public:
	StaticShader()
		: ShaderBase("Shaders/vertexShader.vertexshader", "Shaders/fragmentShader.fragmentshader")
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
		myLocationLightPosition = GetUniformLocation("lighPos");
		myLocationLightColour = GetUniformLocation("lightColour");
		myLocationShineDamper = GetUniformLocation("shineDamper");
		myLocationReflectivity = GetUniformLocation("reflectivity");
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
	void LoadViewMatrix(Camera& aCamera)
	{
		LoadMatrix(myLocationViewMatrix, CreateViewMatrix(aCamera));
	}

	void LoadLight(Light& aLight)
	{
		LoadVector(myLocationLightPosition, aLight.myPosition);
		LoadVector(myLocationLightColour, aLight.myColour);
	}

	void LoadShineVariables(float aDamper, float aReflectivity)
	{
		LoadFloat(myLocationShineDamper, aDamper);
		LoadFloat(myLocationReflectivity, aReflectivity);
	}

private:
	GLuint myLocationTransofmationMatrix;
	GLuint myLocationProjectionMatrix;
	GLuint myLocationViewMatrix;
	GLuint myLocationLightPosition;
	GLuint myLocationLightColour;
	GLuint myLocationShineDamper;
	GLuint myLocationReflectivity;
};