#pragma once

#include <vector>

#include "RawModel.h"
#include "texture.h"

using namespace std;

class Loader
{
public:
	RawModel LoadToVAO(vector<vec3> somePositions, vector<vec2> someUVs, vector<vec3> someNormals, vector<int> someIndices)
	{
		GLuint vaoID = CreateVAO();
		BindIndicesBuffer(someIndices);
		StoreDataInAttributeList(0, 3, somePositions);
		StoreDataInAttributeList(1, 2, someUVs);
		StoreDataInAttributeList(2, 3, someNormals);
		UnbindVAO();

		return RawModel(vaoID, someIndices.size());
	}

	void CleanUp()
	{
		for (GLuint& vao : myVAOs)
			glDeleteVertexArrays(1, &vao);
		for (GLuint& vbo : myVBOs)
			glDeleteBuffers(1, &vbo);
		for(GLuint& texture : myTextures)
			glDeleteTextures(1, &texture);
	}

	GLuint LoadTexture(const char* aFileName)
	{
		GLuint textureID = loadBMP_custom(aFileName);
		myTextures.push_back(textureID);
		return textureID;
	}

private:
	vector<GLuint> myVAOs;
	vector<GLuint> myVBOs;
	vector<GLuint> myTextures;

	GLuint CreateVAO()
	{
		GLuint vaoID;
		glGenVertexArrays(1, &vaoID);
		myVAOs.push_back(vaoID);
		glBindVertexArray(vaoID);
		return vaoID;
	}

	void StoreDataInAttributeList(int anAttributeNumber, int aSize, vector<float> someData)
	{
		GLuint vboID;
		glGenBuffers(1, &vboID);
		myVBOs.push_back(vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, someData.size() * sizeof(float), &someData[0], GL_STATIC_DRAW);
		glVertexAttribPointer(anAttributeNumber, aSize, GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void StoreDataInAttributeList(int anAttributeNumber, int aSize, vector<vec2> someData)
	{
		GLuint vboID;
		glGenBuffers(1, &vboID);
		myVBOs.push_back(vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, someData.size() * sizeof(vec2), &someData[0], GL_STATIC_DRAW);
		glVertexAttribPointer(anAttributeNumber, aSize, GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void StoreDataInAttributeList(int anAttributeNumber, int aSize, vector<vec3> someData)
	{
		GLuint vboID;
		glGenBuffers(1, &vboID);
		myVBOs.push_back(vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, someData.size() * sizeof(vec3), &someData[0], GL_STATIC_DRAW);
		glVertexAttribPointer(anAttributeNumber, aSize, GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void UnbindVAO()
	{
		glBindVertexArray(0);
	}

	void BindIndicesBuffer(vector<int> someIndices)
	{
		GLuint vboID;
		glGenBuffers(1, &vboID);
		myVBOs.push_back(vboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, someIndices.size() * sizeof(int), &someIndices[0], GL_STATIC_DRAW);
	}
};