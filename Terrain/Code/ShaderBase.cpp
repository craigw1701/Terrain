#include "ShaderBase.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <windows.h>
using namespace std;

#include "gl.h"

#include "GameInfo.h"
#include "NonCopyable.h"


vector<ShaderBase*> ShaderBase::ourShaders;

ShaderBase::ShaderBase(const char* aShaderFile) : myShaderName(aShaderFile)
, myProgramID(-1)
{
	char buffer[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, buffer);

	printf("buffer:%s\n", buffer);
	std::string shaderPath = myShaderPath + aShaderFile;
	myProgramID = LoadShaders((shaderPath + ".vertexshader").c_str(), (shaderPath + ".fragmentshader").c_str());
	myTimestamp = GetLastTimeStamp();
	ourShaders.push_back(this);
}

ShaderBase::~ShaderBase()
{
	auto& iter = find(ourShaders.begin(), ourShaders.end(), this);
	if (iter != ourShaders.end())
		ourShaders.erase(iter);

	glDeleteProgram(myProgramID);
}

void ShaderBase::CheckReloadShaders()
{
	for (ShaderBase* shader : ourShaders)
		shader->CheckReloadShader();
}

void ShaderBase::CheckReloadShader()
{
	std::string shaderPath = myShaderPath + myShaderName;
	std::string vertexFile = (shaderPath + ".vertexshader");
	std::string fragmentFile = (shaderPath + ".fragmentshader");


	ULARGE_INTEGER ft = GetLastTimeStamp();

	if ((ft.QuadPart - myTimestamp.QuadPart) > 10000000)
	{
		//cout << "Delta " << (ft.QuadPart - myTimestamp.QuadPart) << endl;
		//char buffer[MAX_PATH];
		//GetCurrentDirectoryA(MAX_PATH, buffer);

		//printf("buffer:%s\n", buffer);
		GLuint programID = LoadShaders(vertexFile.c_str(), fragmentFile.c_str());
		if (programID == -1)
			return;

		if (programID != myProgramID)
		{
			myProgramID = programID;
			myTimestamp = ft;
			Setup();
			Start();
			LoadProjectionMatrix(GameInfo::ourProjectionMatrix);
			ConnectTextureUnits();
			OnReload();
			Stop();
		}
	}
}

void ShaderBase::OnReload()
{

}

void ShaderBase::ConnectTextureUnits()
{

}

void ShaderBase::Setup()
{
	BindAttributes();
	GetUniformLocations();
	myHasSetup = true;
}

void ShaderBase::GetUniformLocations()
{
	myLocationProjectionMatrix = GetUniformLocation("projectionMatrix");
	GetAllUniformLocations();
}

void ShaderBase::LoadProjectionMatrix(glm::mat4 const& aMatrix)
{
	LoadMatrix(myLocationProjectionMatrix, aMatrix);
}

void ShaderBase::Start()
{
	GameInfo::ourCurrentShaderPass = myShaderName;
	assert(myHasSetup); //"not setup");
	glUseProgram(myProgramID);
}

void ShaderBase::Stop()
{
	glUseProgram(0);
	GameInfo::ourCurrentShaderPass = "";
}

GLuint ShaderBase::GetUniformLocation(const char* aUniformName) const
{
	return glGetUniformLocation(myProgramID, aUniformName);
}

void ShaderBase::BindAttribute(GLuint anAttribute, const char* aVariableName)
{
	glBindAttribLocation(myProgramID, anAttribute, aVariableName);
}

void ShaderBase::LoadInt(GLuint aLocation, int aValue)
{
	glUniform1i(aLocation, aValue);
}

void ShaderBase::LoadFloat(GLuint aLocation, float aValue)
{
	glUniform1f(aLocation, aValue);
}

void ShaderBase::LoadVector(GLuint aLocation, vec4 aVector)
{
	glUniform4f(aLocation, aVector.x, aVector.y, aVector.z, aVector.w);
}

void ShaderBase::LoadVector(GLuint aLocation, vec3 aVector)
{
	glUniform3f(aLocation, aVector.x, aVector.y, aVector.z);
}

void ShaderBase::LoadVector(GLuint aLocation, vec2 aVector)
{
	glUniform2f(aLocation, aVector.x, aVector.y);
}

void ShaderBase::LoadBoolean(GLuint aLocation, bool aValue)
{
	glUniform1f(aLocation, aValue ? 1.0f : 0.0f);
}

void ShaderBase::LoadMatrix(GLuint aLocation, glm::mat4x4 aMatrix)
{
	glUniformMatrix4fv(aLocation, 1, GL_FALSE, &aMatrix[0][0]);
}

GLuint ShaderBase::LoadShaders(const char * vertex_file_path, const char * fragment_file_path) const
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	{
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
			char buffer[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, buffer);
			char buffer2[MAX_PATH];
			strerror_s(buffer2, MAX_PATH);
			printf("%s - Impossible to open %s. Are you in the right directory (%s)!\n", buffer2, vertex_file_path, buffer);
			//system("pause");
			return -1;
		}
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	{
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
			char buffer[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, buffer);
			char buffer2[MAX_PATH];
			strerror_s(buffer2, MAX_PATH);
			printf("%s - Impossible to open %s. Are you in the right directory (%s)!\n", buffer2, fragment_file_path, buffer);
			//system("pause");
			return -1;
		}
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

	if (myProgramID != -1)
	{
		glDeleteProgram(myProgramID);
	}

	return ProgramID;
}

ULARGE_INTEGER ShaderBase::GetLastTimeStamp()
{
	std::string shaderPath = myShaderPath + myShaderName;
	std::string vertexFile = (shaderPath + ".vertexshader");
	std::string fragmentFile = (shaderPath + ".fragmentshader");

	ULARGE_INTEGER result = { 0 };
	WIN32_FIND_DATAA vertexFileData;
	
	PVOID OldValue = nullptr;	
	Wow64DisableWow64FsRedirection(&OldValue);
	
	HANDLE vertex = FindFirstFileA(vertexFile.c_str(), &vertexFileData);
	FindClose(vertex);
	if (vertex == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFileA failed with %d\n", GetLastError());
		return result;
	}

	WIN32_FIND_DATAA fragmentFileData;
	HANDLE fragment = FindFirstFileA(fragmentFile.c_str(), &fragmentFileData);
	FindClose(fragment);
	if (fragment == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFileA failed with %d\n", GetLastError());
		return result;
	}

	result.HighPart = vertexFileData.ftLastWriteTime.dwHighDateTime;
	result.LowPart = vertexFileData.ftLastWriteTime.dwLowDateTime;

	ULARGE_INTEGER writeTimeTemp;
	writeTimeTemp.HighPart = fragmentFileData.ftLastWriteTime.dwHighDateTime;
	writeTimeTemp.LowPart = fragmentFileData.ftLastWriteTime.dwLowDateTime;

	result.QuadPart += writeTimeTemp.QuadPart;

	return result;
}
