#pragma once

#include "NonCopyable.h"
#include "Loader.h"
#include "Terrain.h"
#include "TerrainTexture.h"

#include <thread>
#include <mutex>
#include <vector>
#include <sstream>

static std::string locTrim(std::string aString)
{
	std::string s = aString;
	while (s.find(' ') == 0)
	{
		s = s.substr(1);
	}

	while (s.rfind(' ') == s.size()-1)
	{
		s = s.substr(0, s.size() - 1);
		
		if (s.size() == 0)
			return s;
	}
	return s;
}

class TerrainManager : public NonCopyable
{
public:
	TerrainManager(Loader& aLoader, TerrainTexturePack& aTexturePack, TerrainTexture& aBlendMap)
		: myLoader(aLoader)
		, myTexturePack(aTexturePack)
		, myBlendMap(aBlendMap)
	{

		DebugConsole::AddCommand("Terrain.EnableThreaded", "", [](std::string aFullCommand)
		{
			GameInfo::ourGenerateTerrainThreaded = !GameInfo::ourGenerateTerrainThreaded;
			return GameInfo::ourGenerateTerrainThreaded ? "Threaded On" : "Threaded Off";
		});
		DebugConsole::AddCommand("Terrain.EnableCaching", "", [](std::string aFullCommand)
		{
			GameInfo::ourGenerateTerrainCaching = !GameInfo::ourGenerateTerrainCaching;
			return GameInfo::ourGenerateTerrainCaching ? "Caching On" : "Caching Off";
		});
		DebugConsole::AddCommand("Terrain.Regenerate", "", [this](std::string aFullCommand)
		{
			std::string s = locTrim(DebugConsole::GetParam(aFullCommand, 1));
			if (s.size() > 0)
			{
				mySeed = atoi(s.c_str());
			}

			Regenerate(mySeed);
			std::ostringstream stringStream;
			stringStream << "Generating " << (GameInfo::ourGenerateTerrainThreaded ? "threaded" : "single-threaded") << " with seed: " << mySeed;
			return stringStream.str();
		});

#ifdef _DEBUG
		const int numTiles = 1;
#else
		const int numTiles = 4;
#endif
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

	void Regenerate(int aSeed = -1)
	{
		double startTime = glfwGetTime();
		mySeed = aSeed;
		if(mySeed == -1)
			mySeed = rand() % 1000000;

		printf("Starting %s Terrain Generation with seed: %d %s caching\n", 
			GameInfo::ourGenerateTerrainThreaded ? "threaded" : "single-threaded", 
			mySeed,
			GameInfo::ourGenerateTerrainCaching ? "using" : "not using");
		vector<std::thread> threads;
		for (Terrain* terrain : myTerrains)
		{
			if(GameInfo::ourGenerateTerrainThreaded)
				threads.push_back(std::thread(&Terrain::GenerateTerrain, terrain, mySeed));
			else
				terrain->GenerateTerrain(mySeed);
		}

		for(std::thread& t : threads)
			t.join();
		
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