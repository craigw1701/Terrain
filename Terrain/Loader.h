#pragma once

#include <vector>

#include "RawModel.h"

using namespace std;

class Loader
{
public:
	RawModel LoadToVAO(vector<float> somePositions)
	{
		GLuint vaoID = CreateVAO();
		StoreDataInAttributeList(0, somePositions);
		UnbindVAO();

		return RawModel(vaoID, somePositions.size() / 3);
	}

	void CleanUp()
	{
		for (GLuint& vao : myVAOs)
			glDeleteVertexArrays(1, &vao);
		for (GLuint& vbo : myVBOs)
			glDeleteBuffers(1, &vbo);
	}

private:
	vector<GLuint> myVAOs;
	vector<GLuint> myVBOs;

	GLuint CreateVAO()
	{
		GLuint vaoID;
		glGenVertexArrays(1, &vaoID);
		myVAOs.push_back(vaoID);
		glBindVertexArray(vaoID);
		return vaoID;
	}

	void StoreDataInAttributeList(int anAttributeNumber, vector<float> someData)
	{
		GLuint vboID;
		glGenBuffers(1, &vboID);
		myVBOs.push_back(vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, someData.size() * sizeof(float), &someData[0], GL_STATIC_DRAW);
		glVertexAttribPointer(anAttributeNumber, 3, GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void UnbindVAO()
	{
		glBindVertexArray(0);
	}
};