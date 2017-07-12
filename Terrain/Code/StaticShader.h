#pragma once
#include "ShaderBase.h"
#include "Maths.h"
#include "Light.h"

class StaticShader : public ShaderBase
{
public:
	StaticShader()
		: ShaderBase("staticShader")
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
		myLocationFakeLighting = GetUniformLocation("useFakeLighting");
		myLocationPlane = GetUniformLocation("plane");
	}

public:
	void LoadTransformationMatrix(glm::mat4 const& aMatrix)
	{
		LoadMatrix(myLocationTransofmationMatrix, aMatrix);
	}
	void LoadProjectionMatrix(glm::mat4 const& aMatrix)
	{
		LoadMatrix(myLocationProjectionMatrix, aMatrix);
	}
	void LoadViewMatrix(Camera const& aCamera)
	{
		LoadMatrix(myLocationViewMatrix, CreateViewMatrix(aCamera));
	}

	void LoadLight(Light const& aLight)
	{
		LoadVector(myLocationLightPosition, aLight.myPosition);
		LoadVector(myLocationLightColour, aLight.myColour);
	}

	void LoadShineVariables(float aDamper, float aReflectivity)
	{
		LoadFloat(myLocationShineDamper, aDamper);
		LoadFloat(myLocationReflectivity, aReflectivity);
	}

	void LoadFakeLighting(bool aUseFake)
	{
		LoadBoolean(myLocationFakeLighting, aUseFake);
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
	GLuint myLocationFakeLighting;
	GLuint myLocationPlane;
};