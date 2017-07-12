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

	void GetAllUniformLocations() override
	{
		myLocationTranformationMatrix = GetUniformLocation("transformationMatrix");
	}

	void BindAttributes() override
	{
		BindAttribute(0, "position");
	}

private:
	GLuint myLocationTranformationMatrix;
};
