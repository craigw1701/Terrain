#pragma once

#include <random>
#include <map>
#include "PerlinNoise.h"
#include "NonCopyable.h"

class HeightsGenerator : public NonCopyable
{
public:
	HeightsGenerator(int aGridX, int aGridZ, int aVertexCount, int aSeed)
		: myXOffset((aGridX + 2) * (aVertexCount - 1))
		, myZOffset((aGridZ + 2) * (aVertexCount-1))
		, mySeed(aSeed)
		, pn(aSeed)
	{
	}

	void Seed(int aSeed)
	{
		if (aSeed == mySeed)
			return;
		pn = PerlinNoise(aSeed);
		mySeed = aSeed;
		myLookup.clear();
	}

	double GenerateHeight(int aX, int aZ) const
	{
		float x = static_cast<float> (myXOffset + aX);
		float z = static_cast<float>(myZOffset + aZ);

		unsigned int seed = (myXOffset + aX) * 49632 + (myZOffset + aZ) * 325176 + mySeed;
		if (GameInfo::ourGenerateTerrainCaching)
		{
			auto iter = myLookup.find(seed);
			if (iter != myLookup.end())
				return iter->second;
		}
		
		float amplitude = GameInfo::ourHeightInfo.amplitude;
		float frequency = GameInfo::ourHeightInfo.frequency;
		double total = 0;
		for(int i = 0; i < GameInfo::ourHeightInfo.octives; i++)
		{ 
			float sampleX = x * GameInfo::ourHeightInfo.theScale * frequency;
			float sampleZ = z * GameInfo::ourHeightInfo.theScale * frequency;
			total += GetN(sampleX, sampleZ) * amplitude;

			amplitude *= GameInfo::ourHeightInfo.lacunarity;
			frequency *= GameInfo::ourHeightInfo.persistance;
		}

		total *= GameInfo::ourHeightInfo.terrainHeight;
		
		if (GameInfo::ourGenerateTerrainCaching)
			myLookup[seed] = total;

		return total;
	}
	
	double GetN(double x, double z) const
	{
		double n = pn.noise(x, z, 0.8);
	//	n = pow(n, 1.5);
		return n * 2.0 - 1.0f;
	}

private:
	int myXOffset;
	int myZOffset;
	PerlinNoise pn;
	int mySeed;

	mutable std::map<unsigned int, double > myLookup;
};
