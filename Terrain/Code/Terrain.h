#pragma once

#include "NonCopyable.h"
#include "HeightsGenerator.h"
#include "Loader.h"
#include "Maths.h"
#include "RawModel.h"
#include "TerrainTexture.h"

class Terrain : public NonCopyable
{
public:
	Terrain(int aGridX, int aGridZ, Loader& aLoader, TerrainTexturePack& aTexturePack, TerrainTexture& aBlendMap, int aSeed)
		: myTextures(aTexturePack)
		, myBlendMap(aBlendMap)
		, myGenerator(aGridX, aGridZ, ourVertexCount, aSeed)
		, myModel(-1, 0)
		, myX(aGridX * ourSize)
		, myZ(aGridZ * ourSize)
	{
		GenerateTerrain(aLoader);
	}

	float GetHeight(float aWorldX, float aWorldZ) const
	{
		float terrainX = aWorldX - myX;
		float terrainZ = aWorldZ - myZ;

		float gridSquareSize = ourSize / (ourVertexCount - 1);
		int gridX = static_cast<int>(floor(terrainX / gridSquareSize));
		int gridZ = static_cast<int>(floor(terrainZ / gridSquareSize));

		if (gridX >= (ourVertexCount - 1) || gridZ >= (ourVertexCount - 1) || gridX < 0 || gridZ < 0)
		{
			ErrorReturn("Position not on terrain!");
		}

		float xCoord = fmod(terrainX, gridSquareSize) / gridSquareSize;
		float zCoord = fmod(terrainZ, gridSquareSize) / gridSquareSize;

		float answer;
		if (xCoord <= (1 - zCoord)) 
		{
			answer = BarryCentric(
				vec3(0, GetHeight(gridX, gridZ, myGenerator), 0),
				vec3(1, GetHeight(gridX+1, gridZ, myGenerator), 0),
				vec3(0, GetHeight(gridX, gridZ+1, myGenerator), 1),
				vec2(xCoord, zCoord));
		}
		else {
			answer = BarryCentric(
				vec3(1, GetHeight(gridX+1, gridZ, myGenerator), 0),
				vec3(1, GetHeight(gridX+1, gridZ+1, myGenerator), 1),
				vec3(0, GetHeight(gridX, gridZ+1, myGenerator), 1),
				vec2(xCoord, zCoord));
		}

		return answer;
	}
	
	void GenerateTerrain(Loader& aLoader) 
	{
		double startTime = glfwGetTime();
		
		int count = ourVertexCount * ourVertexCount;
		vector<vec3> vertices(count);
		vector<vec3> normals(count);
		vector<vec2> textureCoords(count);
		vector<int> indices(6 * (ourVertexCount - 1)*(ourVertexCount - 1));

		int vertexPointer = 0;
		for (int i = 0; i < ourVertexCount; i++) 
		{
			for (int j = 0; j < ourVertexCount; j++) 
			{
				vertices[vertexPointer].x = (float)j / ((float)ourVertexCount - 1) * ourSize;
				vertices[vertexPointer].y = GetHeight(j, i, myGenerator);
				vertices[vertexPointer].z = (float)i / ((float)ourVertexCount - 1) * ourSize;
				normals[vertexPointer] = GetNormal(j, i, myGenerator);
				textureCoords[vertexPointer].x = (float)j / ((float)ourVertexCount - 1);
				textureCoords[vertexPointer].y = (float)i / ((float)ourVertexCount - 1);
				vertexPointer++;
			}
		}
		int pointer = 0;
		for (int gz = 0; gz < ourVertexCount - 1; gz++)
		{
			for (int gx = 0; gx < ourVertexCount - 1; gx++)
			{
				int topLeft = (gz*ourVertexCount) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1)*ourVertexCount) + gx;
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
		myModel = aLoader.LoadToVAO(vertices, textureCoords, normals, indices);
	}

	float GetX() const { return myX; }
	float GetZ() const { return myZ; }
	RawModel const& GetModel() const { return myModel; }
	TerrainTexturePack const& GetTextures() const { return myTextures; }
	TerrainTexture const& GetBlendMap() const { return myBlendMap; }

	static constexpr float ourSize = 800.0f;
	static const int ourVertexCount = 128;
private:
	vec3 GetNormal(int aX, int aZ, HeightsGenerator const& aGenerator) const
	{
		float heightL = GetHeight(aX - 1, aZ, aGenerator);
		float heightR = GetHeight(aX + 1, aZ, aGenerator);
		float heightD = GetHeight(aX, aZ - 1, aGenerator);
		float heightU = GetHeight(aX, aZ + 1, aGenerator);
		vec3 normal = vec3(heightL - heightR, 2.0f, heightD - heightU);
		normal = glm::normalize(normal);
		return normal;
	}

	float GetHeight(int aX, int aZ, HeightsGenerator const& aGenerator) const
	{
		return aGenerator.GenerateHeight(aX, aZ);
	}

private:
	float myX;
	float myZ;
	HeightsGenerator myGenerator;
	RawModel myModel;
	TerrainTexturePack& myTextures;
	TerrainTexture& myBlendMap;
};
