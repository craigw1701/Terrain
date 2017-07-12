#pragma once

#include "Maths.h"

class WaterTile
{
public:
	WaterTile(vec3 aCenterPos)
		: myCenterPos(aCenterPos)
	{

	}

	float TILE_SIZE = 2000;

	vec3 myCenterPos;
};
