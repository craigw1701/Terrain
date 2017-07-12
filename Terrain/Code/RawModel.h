#pragma once

#include "GL.h"

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