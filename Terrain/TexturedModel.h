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

	RawModel& GetRawModel()		const	{ return myRawModel; }
	ModelTexture& GetTexture()	const	{ return myTexture; }
private:
	RawModel& myRawModel;
	ModelTexture& myTexture;
};