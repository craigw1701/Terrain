#pragma once

#include "HeightsGenerator.h"
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
		double startTime = glfwGetTime();

		std::srand(1);
		HeightsGenerator generator(static_cast<int>(myX/mySize), static_cast<int>(myZ/mySize), myVertexCount, rand() % 1000000);

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
				vertices[vertexPointer].y = GetHeight(j, i, generator);
				vertices[vertexPointer].z = (float)i / ((float)myVertexCount - 1) * mySize;
				normals[vertexPointer] = GetNormal(j, i, generator);
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
		printf("Terrain Generation Time: %f\n", glfwGetTime() - startTime);
		return aLoader.LoadToVAO(vertices, textureCoords, normals, indices);
	}

	vec3 GetNormal(int aX, int aZ, HeightsGenerator& aGenerator)
	{
		float heightL = GetHeight(aX - 1, aZ, aGenerator);
		float heightR = GetHeight(aX + 1, aZ, aGenerator);
		float heightD = GetHeight(aX, aZ - 1, aGenerator);
		float heightU = GetHeight(aX, aZ + 1, aGenerator);
		vec3 normal = vec3(heightL - heightR, 2.0f, heightD - heightU);
		normal = glm::normalize(normal);
		return normal;
	}

	float GetHeight(int aX, int aZ, HeightsGenerator& aGenerator)
	{
		return aGenerator.GenerateHeight(aX, aZ);
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
