#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

#include "ShaderBase.h"


class GUIShader : public ShaderBase
{
public:
	GUIShader()
		: ShaderBase("shaders/gui.vertexshader", "shaders/gui.fragmentshader")
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

	GLuint myLocationTranformationMatrix;
};
