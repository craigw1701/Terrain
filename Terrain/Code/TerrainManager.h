#pragma once

#include "NonCopyable.h"
#include "Loader.h"
#include "StringUtils.h"
#include "Terrain.h"
#include "TerrainTexture.h"

#include <thread>
#include <mutex>
#include <vector>
#include <sstream>
#include <memory>

class TerrainManager : public NonCopyable
{
public:
	TerrainManager(Loader& aLoader, TerrainTexturePack& aTexturePack, TerrainTexture& aBlendMap)
		: myLoader(aLoader)
		, myTexturePack(aTexturePack)
		, myBlendMap(aBlendMap)
	{
		DebugConsole* console = DebugConsole::GetInstance();
		if (console)
		{
			console->AddCommand("Terrain.Regenerate", "", [this](std::vector<std::string> someParams) -> std::string
			{
				if (someParams.size() == 1)
				{
					mySeed = atoi(someParams[0].c_str());
				}
				else if (someParams.size() > 1)
				{
					return std::string("Too many arguments, expects 0 or 1");
				}

				Regenerate(mySeed);
				std::ostringstream stringStream;
				stringStream << "Generating " << (GameInfo::ourGenerateTerrainThreaded ? "threaded" : "single-threaded") << " with seed: " << mySeed;
				return stringStream.str();
			});
			console->AddVariable("Terrain.Threaded", GameInfo::ourGenerateTerrainThreaded);
			console->AddVariable("Terrain.Caching", GameInfo::ourGenerateTerrainCaching);
			console->AddVariable("Terrain.Seed", mySeed);


			console->AddVariable("Terrain.Octives", GameInfo::ourHeightInfo.octives);
			console->AddVariable("Terrain.Amplitude", GameInfo::ourHeightInfo.amplitude);
			console->AddVariable("Terrain.Frequency", GameInfo::ourHeightInfo.frequency);
			console->AddVariable("Terrain.Scale", GameInfo::ourHeightInfo.theScale);
			console->AddVariable("Terrain.Persistance", GameInfo::ourHeightInfo.persistance);
			console->AddVariable("Terrain.Lacunarity", GameInfo::ourHeightInfo.lacunarity);
			console->AddVariable("Terrain.TerrainHeight", GameInfo::ourHeightInfo.terrainHeight);
		}



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
	}

	~TerrainManager()
	{
		myTerrains.clear();
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
		for (auto& terrain : myTerrains)
		{
			if(GameInfo::ourGenerateTerrainThreaded)
				threads.push_back(std::thread(&Terrain::GenerateTerrain, terrain.get(), mySeed));
			else
				terrain->GenerateTerrain(mySeed);
		}

		for(std::thread& t : threads)
			t.join();
		
		for (auto& terrain : myTerrains)
		{
			terrain->Finalize(myLoader);
		}
		printf("-----------------------------------\n");
		printf("Terrain Generation Finished in Time: %f\n", glfwGetTime() - startTime);
		printf("-----------------------------------\n");

	}

	void AddTerrain(int aGridX, int aGridZ)
	{
		std::unique_ptr<Terrain> t = std::make_unique<Terrain>(aGridX, aGridZ, myLoader, myTexturePack, myBlendMap, mySeed);
		unique_lock<mutex> l{ myMutex };
		myTerrains.push_back(std::move(t));
	}

	float GetHeight(float aWorldX, float aWorldZ) const
	{
		int vertexCount = (Terrain::ourVertexCount - 1);
		float gridSquareSize = Terrain::ourSize / vertexCount;

		for (auto const& terrain : myTerrains)
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

	std::vector<std::unique_ptr<Terrain>> const& GetTerrains() const { return myTerrains; }

private:
	Loader& myLoader;
	TerrainTexturePack& myTexturePack;
	TerrainTexture& myBlendMap;
	std::vector<std::unique_ptr<Terrain>> myTerrains;
	int mySeed;

	mutex myMutex;
};