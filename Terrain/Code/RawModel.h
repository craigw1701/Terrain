#pragma once

#include "GL.h"

class RawModel
{
public:
	RawModel(GLuint aVAOID, int aVertexCount, GLuint aVertexID = -1, GLuint aNormalsID = -1, GLuint aUVID = -1)
		: myVAOID(aVAOID)
		, myVertexCount(aVertexCount)
		, myVertexID(aVertexID)
		, myNormalsID(aNormalsID)
		, myUVID(aUVID)
	{}

	GLuint GetVAOID()		const	{ return myVAOID; }
	int GetVertexCount()	const	{ return myVertexCount; }

	GLuint myVertexID;
	GLuint myNormalsID;
	GLuint myUVID;
private:
	GLuint myVAOID;

	int myVertexCount;
};