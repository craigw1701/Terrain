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
		myCurrentRotation += ourRotateSpeed * GameInfo::ourDeltaTime;
		matrix = glm::rotate(matrix, radians(myCurrentRotation), glm::vec3(0, 1, 0));
		LoadMatrix(myLocationViewMatrix, matrix);
	}

	void LoadLight(Light const& aSun)
	{
		LoadVector(myLocationLightColour, aSun.myColour);
		LoadVector(myLocationLightPosition, aSun.myPosition);
		LoadFloat(myLocationDayNightTime, GameInfo::ourDayNightTime);
	}

	void LoadFogColour(vec3 aColour)
	{
		LoadVector(myLocationFogColour, aColour);
	}

	void ConnectTextureUnits()
	{
		LoadInt(myLocationCubeMapDay, 0);
		LoadInt(myLocationCubeMapNight, 1);
	}

	void GetAllUniformLocations() override
	{
		myLocationProjectionMatrix = GetUniformLocation("projectionMatrix");
		myLocationViewMatrix = GetUniformLocation("viewMatrix");
		myLocationLightColour = GetUniformLocation("lightColour");
		myLocationLightPosition = GetUniformLocation("lightPosition");
		myLocationDayNightTime = GetUniformLocation("dayNightTime");
		myLocationCubeMapDay = GetUniformLocation("cubeMap");
		myLocationCubeMapNight = GetUniformLocation("cubeMapNight");
		myLocationFogColour = GetUniformLocation("fogColour");
	}

	void BindAttributes() override
	{
		BindAttribute(0, "position");
	}

private:
	GLuint myLocationProjectionMatrix;
	GLuint myLocationViewMatrix;
	GLuint myLocationLightColour;
	GLuint myLocationDayNightTime;
	GLuint myLocationLightPosition;
	GLuint myLocationCubeMapDay;
	GLuint myLocationCubeMapNight;
	GLuint myLocationFogColour;

	float myCurrentRotation = 0.0f;
	static constexpr float ourRotateSpeed = 0.1f;

};