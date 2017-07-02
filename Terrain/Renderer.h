#pragma once

#include "RawModel.h"
#include "TexturedModel.h"
#include "Entity.h"
#include "StaticShader.h"
#include "Maths.h"

#include "glm/gtx/transform.hpp"

static float locToRadians(float aDegree)
{
	return (aDegree * 3.14f) / 180.0f;
}

class Renderer
{
public:
	Renderer(StaticShader& aShader)
	{
		myProjectionMatrix = glm::perspectiveFov(myFOV, 1024.0f, 768.0f, myNearPlane, myFarPlane);
		aShader.Start();
		aShader.LoadProjectionMatrix(myProjectionMatrix);
		aShader.Stop();
	}
	
	void Prepare()
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);
	}

	void Render(RawModel& aModel)
	{
		glBindVertexArray(aModel.GetVAOID());
		glEnableVertexAttribArray(0);
		//glDrawArrays(GL_TRIANGLES, 0, aModel.GetVertexCount());
		glDrawElements(GL_TRIANGLES, aModel.GetVertexCount(), GL_UNSIGNED_INT, nullptr);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void Render(TexturedModel& aModel)
	{
		glBindVertexArray(aModel.GetRawModel().GetVAOID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, aModel.GetTexture().GetTextureID());
		glDrawElements(GL_TRIANGLES, aModel.GetRawModel().GetVertexCount(), GL_UNSIGNED_INT, nullptr);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}


	void Render(Entity& anEntity, StaticShader& aShader)
	{
		TexturedModel& texturedModel = anEntity.GetModel();
		RawModel& model = texturedModel.GetRawModel();
		glBindVertexArray(model.GetVAOID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glm::mat4 transformationMatrix = CreateTransformMatrix(anEntity.myPosition, anEntity.myRotation, anEntity.myScale);
		aShader.LoadTransformationMatrix(transformationMatrix);

		ModelTexture& texture = texturedModel.GetTexture();
		aShader.LoadShineVariables(texture.myShineDamper, texture.myReflectivity);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());
		glDrawElements(GL_TRIANGLES, model.GetVertexCount(), GL_UNSIGNED_INT, nullptr);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(3);
		glBindVertexArray(0);
	}

private:
	float myFOV = 70;
	float myNearPlane = 0.1f;
	float myFarPlane = 1000.0f;
	glm::mat4 myProjectionMatrix;

};