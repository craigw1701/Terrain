#pragma once

#include "gl.h"

class Light
{
public:
	Light(vec3 aPos, vec3 aColour)
		: myPosition(aPos), myColour(aColour)
	{

	}
	vec3 myPosition;
	vec3 myColour;

};