#pragma once
#include "NonCopyable.h"
#include "ModelTexture.h"
#include "RawModel.h"


class TexturedModel : public NonCopyable
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