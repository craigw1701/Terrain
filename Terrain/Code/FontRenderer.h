#pragma once

#include "FontShader.h"
#include "GUIText.h"

class FontRenderer 
{
private:
	FontShader myShader;

public:
	FontRenderer() 
	{
		myShader.Setup();
	}

	void cleanUp() 
	{
	//	shader.cleanUp();
	}

	void Render(std::map<FontType*, std::vector<GUIText*>>& someTexts)
	{
		Prepare();

		for (auto& iter = someTexts.begin(); iter != someTexts.end(); iter++)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, iter->first->GetTextureAtlas());
			for (GUIText const* text : iter->second)
			{
				RenderText(*text);
			}
		}
		EndRendering();
	}

	void Prepare() 
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		myShader.Start();
	}

	void RenderText(GUIText const& text) 
	{
		glBindVertexArray(text.GetMesh());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		myShader.LoadColour(text.GetColour());
		myShader.LoadTranslation(text.GetPosition());
		glDrawArrays(GL_TRIANGLES, 0, text.GetVertexCount());
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	void EndRendering() 
	{
		myShader.Stop();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

};