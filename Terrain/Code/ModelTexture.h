#pragma once

class ModelTexture
{
public:
	ModelTexture(GLuint anID)
		: myTextureID(anID)
	{

	}

	GLuint GetTextureID()	const	{ return myTextureID;  }

public:
	float myShineDamper = 1;
	float myReflectivity = 0;
	bool myHasTransparency = false;
	bool myUseFakeLighting = false;

private:
	GLuint myTextureID;
};
