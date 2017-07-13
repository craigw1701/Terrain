#pragma once

#include <vector>
#include "NonCopyable.h"
#include "ObjLoader.h"
#include "RawModel.h"
#include "texture.h"

using namespace std;

class Loader : public NonCopyable
{
public:
	Loader()
		: myDataPath("data/")
	{

	}
	RawModel LoadToVAO(vector<vec3> somePositions, vector<vec2> someUVs, vector<vec3> someNormals, vector<int> someIndices)
	{
		GLuint vaoID = CreateVAO();
		BindIndicesBuffer(someIndices);
		GLuint vertexID = StoreDataInAttributeList(0, 3, somePositions);
		GLuint uvID = StoreDataInAttributeList(1, 2, someUVs);
		GLuint normalsID = StoreDataInAttributeList(2, 3, someNormals);
		UnbindVAO();

		return RawModel(vaoID, someIndices.size(), vertexID, normalsID, uvID);
	}

	RawModel LoadToVAO(vector<vec2> somePositions)
	{
		GLuint vaoID = CreateVAO();
		StoreDataInAttributeList(0, 2, somePositions);
		UnbindVAO();
		return RawModel(vaoID, somePositions.size());
	}

	RawModel LoadToVAO(vector<vec3> somePositions)
	{
		GLuint vaoID = CreateVAO();
		StoreDataInAttributeList(0, 3, somePositions);
		UnbindVAO();
		return RawModel(vaoID, somePositions.size());
	}

	RawModel LoadToVAO(const char* anOBJFile)
	{
		std::string fileName;
		std::string extension;
		GetFilePathAndExtension(anOBJFile, fileName, extension);
		if (extension.compare("obj") != 0)
		{
			printf("Failed to load OBJ file: %s", fileName.c_str());
			return RawModel(-1, 0);
		}

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<int> indices;
		LoadOBJ(fileName.c_str(), vertices, uvs, normals, indices);
		return LoadToVAO(vertices, uvs, normals, indices);
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

	GLuint LoadCubeMap(vector<std::string> const& someTextures, GLuint aTextureIndex)
	{
		GLuint textureID;
		glGenTextures(1, &textureID);
		glActiveTexture(aTextureIndex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int i = 0;
		for (string const& texture : someTextures)
		{
			TextureData data = GetTextureData(("data/" + texture).c_str());
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, data.myWidth, data.myHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data.myImageBuffer.front());
			i++;
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		myTextures.push_back(textureID);
		return textureID;
	}
	
	GLuint LoadTexture(const char* aFileName)
	{
		std::string fileName;
		std::string extension;
		GetFilePathAndExtension(aFileName, fileName, extension);

		GLuint textureID = -1;
		if (extension.compare("bmp") == 0)
		{
			textureID = loadBMP_custom(fileName.c_str());
		}
		else if (extension.compare("png") == 0)
		{
			textureID = DecodeOneStep(fileName.c_str());
		}
		else
		{
			printf("Couldn't find a loader for extensions: %s\n", fileName.c_str());
		}

		if (textureID == -1)
		{
			printf("Failed to load file: %s\n", fileName.c_str());
		}
			
		myTextures.push_back(textureID);

		return textureID;
	}

	bool GetFilePathAndExtension(const char* aFileName, std::string& aFilePath, std::string& anExtension)
	{
		aFilePath = myDataPath;
		aFilePath += aFileName;

		int extensionPos = aFilePath.rfind('.');
		if (extensionPos == -1)
		{
			printf("No extension found: %s\n", aFileName);
			return false;
		}

		anExtension = aFilePath.substr(extensionPos + 1);

		return true;
	}

	void UpdateVertexData(vector<vec3> someVertices, vector<vec2> someUVs, vector<vec3> someNormals, RawModel& aModel)
	{
		BindVAO(aModel.GetVAOID());
		UpdateDataInAttributeList(0, 3, someVertices, aModel.myVertexID);
		if(aModel.myUVID != -1)
			UpdateDataInAttributeList(1, 2, someUVs, aModel.myUVID);
		if (aModel.myNormalsID != -1)
			UpdateDataInAttributeList(2, 3, someNormals, aModel.myNormalsID);
		UnbindVAO();
	}

private:
	vector<GLuint> myVAOs;
	vector<GLuint> myVBOs;
	vector<GLuint> myTextures;
	const char* myDataPath;

	GLuint CreateVAO()
	{
		GLuint vaoID;
		glGenVertexArrays(1, &vaoID);
		myVAOs.push_back(vaoID);
		BindVAO(vaoID);
		return vaoID;
	}

	void UpdateDataInAttributeList(int anAttributeNumber, int aSize, vector<float> someData, GLuint aVBOID)
	{
		glBindBuffer(GL_ARRAY_BUFFER, aVBOID);
		glBufferData(GL_ARRAY_BUFFER, someData.size() * sizeof(float), &someData[0], GL_STREAM_DRAW);
		glVertexAttribPointer(anAttributeNumber, aSize, GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void UpdateDataInAttributeList(int anAttributeNumber, int aSize, vector<vec2> someData, GLuint aVBOID)
	{
		glBindBuffer(GL_ARRAY_BUFFER, aVBOID);
		glBufferData(GL_ARRAY_BUFFER, someData.size() * sizeof(vec2), &someData[0], GL_STREAM_DRAW);
		glVertexAttribPointer(anAttributeNumber, aSize, GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void UpdateDataInAttributeList(int anAttributeNumber, int aSize, vector<vec3> someData, GLuint aVBOID)
	{
		glBindBuffer(GL_ARRAY_BUFFER, aVBOID);
		glBufferData(GL_ARRAY_BUFFER, someData.size() * sizeof(vec3), &someData[0], GL_STREAM_DRAW);
		glVertexAttribPointer(anAttributeNumber, aSize, GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLuint StoreDataInAttributeList(int anAttributeNumber, int aSize, vector<float> someData)
	{
		GLuint vboID;
		glGenBuffers(1, &vboID);
		myVBOs.push_back(vboID);
		UpdateDataInAttributeList(anAttributeNumber, aSize, someData, vboID);
		return vboID;
	}
	
	GLuint StoreDataInAttributeList(int anAttributeNumber, int aSize, vector<vec2> someData)
	{
		GLuint vboID;
		glGenBuffers(1, &vboID);
		myVBOs.push_back(vboID);
		UpdateDataInAttributeList(anAttributeNumber, aSize, someData, vboID);
		return vboID;
	}


	GLuint StoreDataInAttributeList(int anAttributeNumber, int aSize, vector<vec3> someData)
	{
		GLuint vboID;
		glGenBuffers(1, &vboID);
		myVBOs.push_back(vboID);
		UpdateDataInAttributeList(anAttributeNumber, aSize, someData, vboID);
		return vboID;
	}

	void BindVAO(GLuint aVAO)
	{
		glBindVertexArray(aVAO);
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