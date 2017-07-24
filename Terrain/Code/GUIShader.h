#pragma once

#include "GL.h"

#include "ShaderBase.h"


class GUIShader : public ShaderBase
{
public:
	GUIShader()
		: ShaderBase("gui")
	{

	}

	void LoadTransformationMatrix(mat4 aMatrix)
	{
		LoadMatrix(myLocationTranformationMatrix, aMatrix);
	}

	void LoadIsDepthTexture(bool aIsDepth)
	{
		LoadBoolean(myLocationIsDepthTexture, aIsDepth);
	}

	void GetAllUniformLocations() override
	{
		myLocationTranformationMatrix = GetUniformLocation("transformationMatrix");
		myLocationIsDepthTexture = GetUniformLocation("isDepthTexture");
	}

	void BindAttributes() override
	{
		BindAttribute(0, "position");
	}

private:
	GLuint myLocationTranformationMatrix;
	GLuint myLocationIsDepthTexture;
};
