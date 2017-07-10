#pragma once

#include "Maths.h"

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
		glDeleteTextures(1, &myReflectionTexture);
		glDeleteRenderbuffers(1, &myReflectionDepthBuffer);

		glDeleteFramebuffers(1, &myRefractionFrameBuffer);
		glDeleteTextures(1, &myRefractionTexture);
		glDeleteTextures(1, &myRefractionDepthTexture);
	}

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

	GLuint CreateTextureAttachment(int width, int height) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
		return texture;
	}

	GLuint CreateDepthBufferAttachment(int width, int height) {
		GLuint depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, depthBuffer);
		return depthBuffer;
	}

	GLuint CreateDepthTextureAttachment(int width, int height) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
		return texture;
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

	void BindFrameBuffer(int frameBuffer, int width, int height) {
		glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glViewport(0, 0, width, height);
	}

	void UnbindCurrentFrameBuffer() //call to switch to default frame buffer
	{
		GameInfo::ourCurrentRenderPass = "Main";
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, GameInfo::ourScreenWidth, GameInfo::ourScreenHeight);
	}

	int REFLECTION_WIDTH = 320;
	int REFLECTION_HEIGHT = 180;

	int REFRACTION_WIDTH = 1280;
	int REFRACTION_HEIGHT = 720;

	GLuint myReflectionFrameBuffer;
	GLuint myReflectionTexture;
	GLuint myReflectionDepthBuffer;

	GLuint myRefractionFrameBuffer;
	GLuint myRefractionTexture;
	GLuint myRefractionDepthTexture;
};