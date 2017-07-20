#pragma once

#include <windows.h>
#include <string>
#include <vector>

#include "gl.h"

#include "NonCopyable.h"


class ShaderBase : public NonCopyable
{
	bool myHasSetup = false;
	std::string myShaderPath = "Shaders/";
	std::string myShaderName;
	GLuint myProgramID;
	ULARGE_INTEGER myTimestamp;

	static std::vector<ShaderBase*> ourShaders;
public:
	ShaderBase(const char* aShaderFile);
	virtual ~ShaderBase();	

	static void CheckReloadShaders();
	void Setup();
	void Start();
	void Stop();

	void LoadProjectionMatrix(glm::mat4 const& aMatrix);

	virtual void ConnectTextureUnits(); // TODO:CW Call from setup?
protected:
	virtual void GetAllUniformLocations() = 0;
	virtual void OnReload();
	virtual void BindAttributes() = 0;

	GLuint GetUniformLocation(const char* aUniformName) const;
	void BindAttribute(GLuint anAttribute, const char* aVariableName);

	void LoadBoolean(GLuint aLocation, bool aValue);
	void LoadInt(GLuint aLocation, int aValue);
	void LoadFloat(GLuint aLocation, float aValue);
	void LoadVector(GLuint aLocation, vec2 aVector);
	void LoadVector(GLuint aLocation, vec3 aVector);
	void LoadVector(GLuint aLocation, vec4 aVector);
	void LoadMatrix(GLuint aLocation, glm::mat4x4 aMatrix);

private:
	void GetUniformLocations();

	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) const;
	void CheckReloadShader();
	ULARGE_INTEGER GetLastTimeStamp();

	GLuint myLocationProjectionMatrix;

};
