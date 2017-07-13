#pragma once

#include "GL.h"

class RawModel
{
public:
	RawModel(GLuint aVAOID, int aVertexCount, GLuint aVertexID = -1) : myVAOID(aVAOID), myVertexCount(aVertexCount), myVertexID(aVertexID){}

	GLuint GetVAOID()		const	{ return myVAOID; }
	int GetVertexCount()	const	{ return myVertexCount; }
	GLuint myVertexID;
private:
	GLuint myVAOID;

	int myVertexCount;
};