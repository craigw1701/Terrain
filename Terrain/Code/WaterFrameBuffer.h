#pragma once

#include "Maths.h"

struct FBO
{
	enum class FBOType
	{
		DEPTH_TEXTURE,
		DEPTH_BUFFER,
		COLOUR_TEXTURE,
		NONE
	};
	FBO() :myFBOID(-1), myType(FBOType::NONE) {}
	FBO(GLuint aFBOID, FBOType aType)
		: myFBOID(aFBOID)
		, myType(aType)
	{
		ourFBOs.push_back(*this);
	}

	friend bool operator==(FBO const& aLHS, FBO const& aRHS)
	{
		return aLHS.myFBOID == aRHS.myFBOID && aLHS.myType == aRHS.myType;
	}

	void Cleanup()
	{
		auto& iter = find(ourFBOs.begin(), ourFBOs.end(), *this);
		if (iter != ourFBOs.end())
			ourFBOs.erase(iter);

		if(myType == FBOType::DEPTH_BUFFER)
			glDeleteRenderbuffers(1, &myFBOID);
		else if(myType != FBOType::NONE)
			glDeleteTextures(1, &myFBOID);
	}

	GLuint myFBOID;
	FBOType myType;
	static vector<FBO> ourFBOs;
};

__declspec(selectany) vector<FBO> FBO::ourFBOs;

class WaterFrameBuffer
{
public:
	WaterFrameBuffer()
	{
		InitialiseReflectionFrameBuffer();
		InitialiseRefractionFrameBuffer();
	}

	~WaterFrameBuffer()
	{
		glDeleteFramebuffers(1, &myReflectionFrameBuffer);
		myReflectionTexture.Cleanup();
		myReflectionDepthBuffer.Cleanup();

		glDeleteFramebuffers(1, &myRefractionFrameBuffer);
		myRefractionTexture.Cleanup();
		myRefractionDepthTexture.Cleanup();
	}

	void BindReflectionFrameBuffer() //call before rendering to this FBO
	{
		GameInfo::ourCurrentRenderPass = "Reflection";
		BindFrameBuffer(myReflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
	}

	void BindRefractionFrameBuffer() //call before rendering to this FBO
	{
		GameInfo::ourCurrentRenderPass = "Refraction";
		BindFrameBuffer(myRefractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
	}

	void UnbindCurrentFrameBuffer() //call to switch to default frame buffer
	{
		GameInfo::ourCurrentRenderPass = "Main";
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, GameInfo::ourScreenWidth, GameInfo::ourScreenHeight);
	}

private:
	void InitialiseReflectionFrameBuffer()
	{
		myReflectionFrameBuffer = CreateFrameBuffer();
		myReflectionTexture = CreateTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
		myReflectionDepthBuffer = CreateDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
		UnbindCurrentFrameBuffer();
	}

	void InitialiseRefractionFrameBuffer()
	{
		myRefractionFrameBuffer = CreateFrameBuffer();
		myRefractionTexture = CreateTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
		myRefractionDepthTexture = CreateDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
		UnbindCurrentFrameBuffer();
	}

	GLuint CreateFrameBuffer()
	{
		GLuint frameBuffer;
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		return frameBuffer;
	}

	FBO CreateTextureAttachment(int width, int height) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
		FBO fbo(texture, FBO::FBOType::COLOUR_TEXTURE);
		return fbo;
	}

	FBO CreateDepthBufferAttachment(int width, int height) {
		GLuint depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, depthBuffer);
		
		FBO fbo(depthBuffer, FBO::FBOType::DEPTH_BUFFER);
		return fbo;
	}

	FBO CreateDepthTextureAttachment(int width, int height) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);

		FBO fbo(texture, FBO::FBOType::DEPTH_TEXTURE);
		return fbo;
	}


	void BindFrameBuffer(int frameBuffer, int width, int height) {
		glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glViewport(0, 0, width, height);
	}

public:
	int REFLECTION_WIDTH = 1280;
	int REFLECTION_HEIGHT = 720;

	int REFRACTION_WIDTH = 1280;
	int REFRACTION_HEIGHT = 720;

	GLuint myReflectionFrameBuffer;
	FBO myReflectionTexture;
	FBO myReflectionDepthBuffer;

	GLuint myRefractionFrameBuffer;
	FBO myRefractionTexture;
	FBO myRefractionDepthTexture;
};