#pragma once

#include "RawModel.h"

class Renderer
{
public:
	void Prepare()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1, 0, 0, 1);
	}

	void Render(RawModel& aModel)
	{
		glBindVertexArray(aModel.GetVAOID());
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, aModel.GetVertexCount());
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
};