#pragma once

#include "NonCopyable.h"
#include "Loader.h"
#include "Terrain.h"
#include "TerrainTexture.h"

#include <thread>
#include <mutex>
#include <vector>

class TerrainManager : public NonCopyable
{
public:
	TerrainManager(Loader& aLoader, TerrainTexturePack& aTexturePack, TerrainTexture& aBlendMap)
		: myLoader(aLoader)
		, myTexturePack(aTexturePack)
		, myBlendMap(aBlendMap)
	{
		std::srand(1);
		mySeed = rand() % 1000000;
		// TODO:CW spin up threads to generate 
		for (int i = -2; i < 2; i++)
		{
			for (int j = -2; j < 2; j++)
			{
				AddTerrain(i, j);
			}
		}
	}

	~TerrainManager()
	{
		for (Terrain* terrain : myTerrains)
		{
			delete terrain;
		}
	}

	void AddTerrain(int aGridX, int aGridZ)
	{
		Terrain* t = new Terrain(aGridX, aGridZ, myLoader, myTexturePack, myBlendMap, mySeed);
		unique_lock<mutex> l{ myMutex };
		myTerrains.push_back(t);
	}

	float GetHeight(float aWorldX, float aWorldZ) const
	{
		int vertexCount = (Terrain::ourVertexCount - 1);
		float gridSquareSize = Terrain::ourSize / vertexCount;

		for (Terrain const* terrain : myTerrains)
		{
			float terrainX = aWorldX - terrain->GetX();
			float terrainZ = aWorldZ - terrain->GetZ();

			int gridX = static_cast<int>(floor(terrainX / gridSquareSize));
			int gridZ = static_cast<int>(floor(terrainZ / gridSquareSize));

			if (gridX >= vertexCount || gridZ >= vertexCount || gridX < 0 || gridZ < 0)
			{
				continue;
			}

			return terrain->GetHeight(aWorldX, aWorldZ);
		}
		//ErrorReturn("Failed to find position on any terrain");
		return GameInfo::ourWaterHeight;
	}

	std::vector<Terrain*> const& GetTerrains() const { return myTerrains; }
private:
	Loader& myLoader;
	TerrainTexturePack& myTexturePack;
	TerrainTexture& myBlendMap;
	std::vector<Terrain*> myTerrains;
	int mySeed;

	mutex myMutex;
};