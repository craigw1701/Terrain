#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include "GL\glew.h"

// Include GLFW
#include "GL\glfw3.h"

// Include GLM
#include "glm/glm.hpp"
using namespace glm;

class ShaderBase
{
	bool myHasSetup = false;
	std::string myShaderPath = "Shaders/";
	std::string myShaderName;
public:
	ShaderBase(const char* aShaderFile)
		: myShaderName(aShaderFile)
	{
		std::string shaderPath = myShaderPath + aShaderFile;
		myProgramID = LoadShaders((shaderPath + ".vertexshader").c_str(), (shaderPath + ".fragmentshader").c_str());
	}

	virtual ~ShaderBase()
	{
		glDeleteProgram(myProgramID);
	}

	void Setup()
	{
		BindAttributes();
		GetAllUniformLocations();
		myHasSetup = true;
	}

	void Start()
	{
		GameInfo::ourCurrentShaderPass = myShaderName;
		assert(myHasSetup); //"not setup");
		glUseProgram(myProgramID);
	}

	void Stop()
	{
		glUseProgram(0);
		GameInfo::ourCurrentShaderPass = "";
	}

protected:
	virtual void GetAllUniformLocations() = 0;

	GLuint GetUniformLocation(const char* aUniformName) const
	{
		return glGetUniformLocation(myProgramID, aUniformName);
	}

	virtual void BindAttributes() = 0;

	void BindAttribute(GLuint anAttribute, const char* aVariableName)
	{
		glBindAttribLocation(myProgramID, anAttribute, aVariableName);
	}

	void LoadInt(GLuint aLocation, int aValue)
	{
		glUniform1i(aLocation, aValue);
	}

	void LoadFloat(GLuint aLocation, float aValue)
	{
		glUniform1f(aLocation, aValue);
	}

	void LoadVector(GLuint aLocation, vec3 aVector)
	{
		glUniform3f(aLocation, aVector.x, aVector.y, aVector.z);
	}

	void LoadVector(GLuint aLocation, vec4 aVector)
	{
		glUniform4f(aLocation, aVector.x, aVector.y, aVector.z, aVector.w);
	}

	void LoadBoolean(GLuint aLocation, bool aValue)
	{
		glUniform1f(aLocation, aValue ? 1.0f : 0.0f);
	}

	void LoadMatrix(GLuint aLocation, glm::mat4x4 aMatrix)
	{
		glUniformMatrix4fv(aLocation, 1, GL_FALSE, &aMatrix[0][0]);
	}

private:
	GLuint myProgramID;
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) const
	{

		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
		if (VertexShaderStream.is_open()) 
		{
			std::string Line = "";
			while (getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();
		}
		else 
		{
			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
			getchar();
			return 0;
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
		if (FragmentShaderStream.is_open()) 
		{
			std::string Line = "";
			while (getline(FragmentShaderStream, Line))
				FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}
		else 
		{
			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", fragment_file_path);
			getchar();
			return 0;
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;


		// Compile Vertex Shader
		printf("Compiling shader : %s\n", vertex_file_path);
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}



		// Compile Fragment Shader
		printf("Compiling shader : %s\n", fragment_file_path);
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}



		// Link the program
		printf("Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}


		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
	}


};
