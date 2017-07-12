#pragma once

#include "GL.h"

#include "Loader.h"

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

	TerrainTexturePack(const char* aBackgroundTexture, const char* aRTexture, const char* aGTexture, const char* aBTexture, Loader& aLoader)
		:TerrainTexturePack(aLoader.LoadTexture(aBackgroundTexture), aLoader.LoadTexture(aRTexture), aLoader.LoadTexture(aGTexture), aLoader.LoadTexture(aBTexture))
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