#pragma once

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include "GL.h"

#pragma warning(disable:4996)

//#include "objloader.hpp"

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

static bool LoadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	std::vector<int> & out_indices
) 
{
	printf("Loading OBJ file %s...\n", path);

	out_vertices.clear();
	out_uvs.clear();
	out_normals.clear();
	out_indices.clear();

 	std::vector<glm::vec2> temp_uvs;
 	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;


	FILE * file = fopen(path, "r");
	if (file == NULL) 
	{
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		//system("pause");
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

				   // else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			out_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			if (uvs.size() == 0)
			{
				out_uvs.resize(out_vertices.size());
				out_normals.resize(out_vertices.size());
			}

			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}

			out_uvs[vertexIndex[0] - 1] = temp_uvs[uvIndex[0] - 1];
			out_uvs[vertexIndex[1] - 1] = temp_uvs[uvIndex[1] - 1];
			out_uvs[vertexIndex[2] - 1] = temp_uvs[uvIndex[2] - 1];

			out_uvs[vertexIndex[0] - 1].y = 1 - out_uvs[vertexIndex[0] - 1].y;
			out_uvs[vertexIndex[1] - 1].y = 1 - out_uvs[vertexIndex[1] - 1].y;
			out_uvs[vertexIndex[2] - 1].y = 1 - out_uvs[vertexIndex[2] - 1].y;

			out_normals[vertexIndex[0] - 1] = temp_normals[normalIndex[0] - 1];
			out_normals[vertexIndex[1] - 1] = temp_normals[normalIndex[1] - 1];
			out_normals[vertexIndex[2] - 1] = temp_normals[normalIndex[2] - 1];

			out_indices.push_back(vertexIndex[0] - 1);
			out_indices.push_back(vertexIndex[1] - 1);
			out_indices.push_back(vertexIndex[2] - 1);
			/*uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);*/
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}
	/*
	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}*/

	return true;
}