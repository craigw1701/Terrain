#pragma once

#include "gl.h"

using namespace glm;

class GUITexture
{
public:
	GUITexture(GLuint aTextureID, vec2 aPosition, vec2 aScale, bool aIsDepth = false)
		: myTextureID(aTextureID)
		, myPosition(aPosition)
		, myScale(aScale)
		, myIsDepth(aIsDepth)
	{}
	~GUITexture() = default;

	GLuint myTextureID;
	vec2 myPosition;
	vec2 myScale;
	bool myIsDepth;
};
