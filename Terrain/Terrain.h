#pragma once

#include "Loader.h"
#include "RawModel.h"
#include "TerrainTexture.h"

class Terrain
{
public:
	Terrain(int aGridX, int aGridZ, Loader& aLoader, TerrainTexturePack& aTexturePack, TerrainTexture& aBlendMap)
		: myTextures(aTexturePack)
		, myBlendMap(aBlendMap)
		, myModel(GenerateTerrain(aLoader))
		, myX(aGridX * mySize)
		, myZ(aGridZ * mySize)
	{
	}

	RawModel GenerateTerrain(Loader& aLoader) 
	{
		int count = myVertexCount * myVertexCount;
		vector<vec3> vertices(count);
		vector<vec3> normals(count);
		vector<vec2> textureCoords(count);
		vector<int> indices(6 * (myVertexCount - 1)*(myVertexCount - 1));

		int vertexPointer = 0;
		for (int i = 0; i<myVertexCount; i++) 
		{
			for (int j = 0; j<myVertexCount; j++) 
			{
				vertices[vertexPointer].x = (float)j / ((float)myVertexCount - 1) * mySize;
				vertices[vertexPointer].y = 0;
				vertices[vertexPointer].z = (float)i / ((float)myVertexCount - 1) * mySize;
				normals[vertexPointer].x = 0;
				normals[vertexPointer].y = 1;
				normals[vertexPointer].z = 0;
				textureCoords[vertexPointer].x = (float)j / ((float)myVertexCount - 1);
				textureCoords[vertexPointer].y = (float)i / ((float)myVertexCount - 1);
				vertexPointer++;
			}
		}
		int pointer = 0;
		for (int gz = 0; gz<myVertexCount - 1; gz++) 
		{
			for (int gx = 0; gx<myVertexCount - 1; gx++) 
			{
				int topLeft = (gz*myVertexCount) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1)*myVertexCount) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}
		return aLoader.LoadToVAO(vertices, textureCoords, normals, indices);
	}

	float GetX() const { return myX; }
	float GetZ() const { return myZ; }
	RawModel const& GetModel() const { return myModel; }
	TerrainTexturePack const& GetTextures() const { return myTextures; }
	TerrainTexture const& GetBlendMap() const { return myBlendMap; }

private:
	float mySize = 800;
	int myVertexCount = 128;

	float myX;
	float myZ;
	RawModel myModel;
	TerrainTexturePack& myTextures;
	TerrainTexture& myBlendMap;
};
