#pragma once

#include "ShaderBase.h"

class SunShader : public ShaderBase
{
	GLuint myLocationTexture;
	GLuint myLocationMVPMatrix;
	GLuint myLocationSunColour;

public:
	SunShader()
		:ShaderBase("sun")
	{
	}

	void LoadTransformationMatrix(mat4 aMatrix)
	{
		LoadMatrix(myLocationMVPMatrix, aMatrix);
	}

	void ConnectTextureUnits()
	{
		LoadInt(myLocationTexture, 0);
	}

	void LoadSunColour(vec3 aColour)
	{
		LoadVector(myLocationSunColour, aColour);
	}

protected:
	void GetAllUniformLocations() override
	{
		myLocationMVPMatrix = GetUniformLocation("mvpMatrix");
		myLocationTexture = GetUniformLocation("sunTexture");
		myLocationSunColour = GetUniformLocation("sunColour");
	}

	void BindAttributes() override
	{
		BindAttribute(0, "position");
	}
};