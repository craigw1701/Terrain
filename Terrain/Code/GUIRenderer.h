#pragma once

#include "GUIShader.h"
#include "GUITexture.h"
#include "Loader.h"
#include "Maths.h"
#include "RawModel.h"

class GUIRenderer
{
public:
	GUIRenderer(Loader &aLoader)
		: myQuad(aLoader.LoadToVAO(positions))
	{		
	}

	void Setup()
	{
		myShader.Setup();
	}

	void Render(vector<GUITexture>& someTextures)
	{
		myShader.Start();
		glBindVertexArray(myQuad.GetVAOID());
		glEnableVertexAttribArray(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		for (GUITexture& gui : someTextures)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gui.myTextureID);
			mat4 matrix = CreateTransformationMatrix(gui.myPosition, gui.myScale);
			myShader.LoadTransformationMatrix(matrix);
			GameInfo::SetRenderTriangles(myQuad.GetVertexCount() - 2);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, myQuad.GetVertexCount());
		}
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		myShader.Stop();
	}

private:
	vector<vec2> positions{ { -1, 1 },{ -1, -1 },{ 1, 1 },{ 1, -1 } };
	RawModel myQuad;
	GUIShader myShader;

};