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
		const int numTiles = 4;
		for (int i = -numTiles; i < numTiles; i++)
		{
			for (int j = -numTiles; j < numTiles; j++)
			{
				AddTerrain(i, j);
			}
		}

		Regenerate();
	}

	~TerrainManager()
	{
		for (Terrain* terrain : myTerrains)
		{
			delete terrain;
		}
	}

	void Regenerate()
	{
		double startTime = glfwGetTime();
		mySeed = rand() % 1000000;
		printf("Starting %s Terrain Generation with seed: %d\n", GameInfo::ourGenerateTerrainThreaded ? "threaded" : "single-threaded", mySeed);
		vector<std::thread> threads;
		for (Terrain* terrain : myTerrains)
		{
			if(GameInfo::ourGenerateTerrainThreaded)
				threads.push_back(std::thread(&Terrain::GenerateTerrain, terrain, mySeed));
			else
				terrain->GenerateTerrain(mySeed);
		}


		float time = glfwGetTime();
		for(std::thread& t : threads)
			t.join();
		printf("Theads finished in: %f\n", glfwGetTime() - time);

		for (Terrain* terrain : myTerrains)
		{
			terrain->Finalize(myLoader);
		}
		printf("-----------------------------------\n");
		printf("Terrain Generation Finished in Time: %f\n", glfwGetTime() - startTime);
		printf("-----------------------------------\n");

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