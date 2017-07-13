#pragma once

#include <random>
#include <map>
#include "PerlinNoise.h"
#include "NonCopyable.h"

#define AMPLITUTE 70.0f

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

		double total = GetN(x, z) * AMPLITUTE * 5;
		total += GetN(x * 4.0f, z * 4.0f) * AMPLITUTE / 2.0f;
		total += GetN(x * 8.0f, z * 8.0f) * AMPLITUTE / 16.0f;
		
		if (GameInfo::ourGenerateTerrainCaching)
			myLookup[seed] = total;

		return total;
	}
	
	double GetN(double x, double z) const
	{
		double n = pn.noise(x / 64.0f, z / 64, 0.8);
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
