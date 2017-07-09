#pragma once

#include <random>
#include <map>

#define AMPLITUTE 70.0f

static unsigned int g_seed;

// Used to seed the generator.           
inline void fast_srand(int seed) {
	g_seed = seed;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
inline int fast_rand(void) {
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}

class HeightsGenerator
{
public:
	HeightsGenerator(int aGridX, int aGridZ, int aVertexCount, int aSeed)
		: myXOffset((aGridX + 2) * (aVertexCount - 1))
		, myZOffset((aGridZ + 2) * (aVertexCount-1))
		, mySeed(aSeed)
	{
	}

	int myXOffset;
	int myZOffset;

	float GenerateHeight(int aX, int aZ)
	{
		int x = myXOffset + aX;
		int z = myZOffset + aZ;

		float total = GetInterpolatedNoise(x / 8.0f, z / 8.0f) * AMPLITUTE * 2.0f;
		total += GetInterpolatedNoise(x / 2.0f, z / 2.0f) * AMPLITUTE / 6.0f;
		total += GetInterpolatedNoise(x, z) * AMPLITUTE / 12.0f;
		return total;
	}

	float GetInterpolatedNoise(float aX, float aZ)
	{
		int intX = (int)aX;
		int intZ = (int)aZ;

		float fracX = aX - intX;
		float fracZ = aZ - intZ;

		float v1 = GetSmoothNoise(intX, intZ);
		float v2 = GetSmoothNoise(intX+1, intZ);
		float v3 = GetSmoothNoise(intX, intZ+1);
		float v4 = GetSmoothNoise(intX+1, intZ+1);

		float i1 = Interpolate(v1, v2, fracX);
		float i2 = Interpolate(v3, v4, fracX);

		return Interpolate(i1, i2, fracZ);
	}

	float Interpolate(float anA, float aB, float aBlend)
	{
		double theata = aBlend * 3.14f;
		float f = (float)(1.0f - cos(theata)) * 0.5f;
		return anA * (1.0f - f) + aB * f;
	}

	float GetSmoothNoise(int aX, int aZ)
	{
		float corners = (GetNoise(aX-1, aZ-1) + GetNoise(aX+1, aZ-1) + GetNoise(aX-1, aZ+1) + GetNoise(aX+1, aZ+1)) / 16.0f;
		float sides = (GetNoise(aX - 1, aZ) + GetNoise(aX + 1, aZ) + GetNoise(aX, aZ - 1) + GetNoise(aX, aZ + 1)) / 8.0f;

		float center = GetNoise(aX, aZ) / 4.0f;

		return corners + sides + center;
	}

	float GetNoise(int aX, int aZ)
	{

		unsigned int seed = aX * 49632 + aZ * 325176 + mySeed;
			
#ifdef _DEBUG
		fast_srand(seed);
		float r = static_cast <float> (fast_rand()) / static_cast <float> (32767);
		
#else
		auto iter = myLookup.find(seed);
		if (iter != myLookup.end())
			return iter->second;
		ra.seed(seed);
		float r = static_cast <float> (ra()) / static_cast <float> (ra.max());
#endif
		r = r * 2.0f;
		r = r - 1.0f;

#ifndef _DEBUG
		myLookup[seed] = r;
#endif
		return r;
	}

	int mySeed;
	std::mt19937 ra;
	std::map<unsigned int, float> myLookup;
	
};
