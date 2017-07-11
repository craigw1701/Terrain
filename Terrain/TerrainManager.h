#pragma once

#include "Loader.h"
#include "Terrain.h"
#include "TerrainTexture.h"

#include <vector>

class TerrainManager
{
public:
	TerrainManager(Loader& aLoader, TerrainTexturePack& aTexturePack, TerrainTexture& aBlendMap)
		: myLoader(aLoader)
		, myTexturePack(aTexturePack)
		, myBlendMap(aBlendMap)
	{
		AddTerrain(-1, -1);
		AddTerrain(0, -1);
		AddTerrain(0, 0);
		AddTerrain(-1, 0);
	}

	void AddTerrain(int aGridX, int aGridZ)
	{
		myTerrains.push_back(Terrain(aGridX, aGridZ, myLoader, myTexturePack, myBlendMap));
	}

	float GetHeight(float aWorldX, float aWorldZ) const
	{
		int vertexCount = (Terrain::ourVertexCount - 1);
		float gridSquareSize = Terrain::ourSize / vertexCount;

		for (Terrain const& terrain : myTerrains)
		{
			float terrainX = aWorldX - terrain.GetX();
			float terrainZ = aWorldZ - terrain.GetZ();

			int gridX = static_cast<int>(floor(terrainX / gridSquareSize));
			int gridZ = static_cast<int>(floor(terrainZ / gridSquareSize));

			if (gridX >= vertexCount || gridZ >= vertexCount || gridX < 0 || gridZ < 0)
			{
				continue;
			}

			return terrain.GetHeight(aWorldX, aWorldZ);
		}
		//ErrorReturn("Failed to find position on any terrain");
		return GameInfo::ourWaterHeight;
	}

	std::vector<Terrain> const& GetTerrains() const { return myTerrains; }
private:
	Loader& myLoader;
	TerrainTexturePack& myTexturePack;
	TerrainTexture& myBlendMap;
	std::vector<Terrain> myTerrains;
};