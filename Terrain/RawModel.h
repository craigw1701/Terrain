#pragma once

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

class RawModel
{
public:
	RawModel(GLuint aVAOID, int aVertexCount) : myVAOID(aVAOID), myVertexCount(aVertexCount) {}

	GLuint GetVAOID()		const	{ return myVAOID; }
	int GetVertexCount()	const	{ return myVertexCount; }
private:
	GLuint myVAOID;
	int myVertexCount;
};