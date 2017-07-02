#pragma once
#include "ModelTexture.h"
#include "RawModel.h"


class TexturedModel
{
public:
	TexturedModel(RawModel& aRawModel, ModelTexture& aTexture)
		: myRawModel(aRawModel), myTexture(aTexture)
	{
	}

	RawModel& GetRawModel() { return myRawModel; }
	ModelTexture& GetTexture() { return myTexture; }
private:
	RawModel& myRawModel;
	ModelTexture& myTexture;
};