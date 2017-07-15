#pragma once

#include "ShaderBase.h"

class FontShader : public ShaderBase
{
private:
	GLuint myLocationColour;
	GLuint myLocationTranslation;
public:
	FontShader() 
		:ShaderBase("font")
	{
	}

	void GetAllUniformLocations() override
	{
		myLocationColour = GetUniformLocation("colour");
		myLocationTranslation = GetUniformLocation("translation");
	}

	void BindAttributes() override
	{
		BindAttribute(0, "position");
		BindAttribute(1, "textureCoords");
	}

	void LoadColour(vec3 aColour)
	{
		LoadVector(myLocationColour, aColour);
	}

	void LoadTranslation(vec2 aTranslation)
	{
		LoadVector(myLocationTranslation, aTranslation);
	}


};