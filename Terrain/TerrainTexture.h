#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

class TerrainTexture
{
public:
	TerrainTexture(GLuint aTextureID)
		: myTextureID(aTextureID)
	{}

	GLuint myTextureID;
};

class TerrainTexturePack
{
public:

	TerrainTexturePack(const char* aBackgroundTexture, const char* aRTexture, const char* aGTexture, const char* aBTexture)
		:TerrainTexturePack(DecodeOneStep(aBackgroundTexture), DecodeOneStep(aRTexture), DecodeOneStep(aGTexture), DecodeOneStep(aBTexture))
	{
	}

	TerrainTexturePack(TerrainTexture aBackgroundTexture, TerrainTexture aRTexture, TerrainTexture aGTexture, TerrainTexture aBTexture)
		: myBackgroundTexture(aBackgroundTexture)
		, myRTexture(aRTexture)
		, myGTexture(aGTexture)
		, myBTexture(aBTexture)
	{}

	TerrainTexture myBackgroundTexture;
	TerrainTexture myRTexture;
	TerrainTexture myGTexture;
	TerrainTexture myBTexture;
};