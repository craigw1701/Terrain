#include "FontType.h"

#include "GUIText.h"
#include "TextMeshCreator.h"
#include "TextMeshData.h"

FontType::FontType(int aTextureAtlas, const char* aFontFile)
{
	myTextureAtlas = aTextureAtlas;
	myLoader = new TextMeshCreator(aFontFile);
}

FontType::~FontType()
{
	// delete myLoader; // TODO:CW Fix crash
	myLoader = nullptr;
}

int FontType::GetTextureAtlas() const
{
	return myTextureAtlas;
}

TextMeshData FontType::LoadText(GUIText* aText)
{
	return myLoader->CreateTextMesh(aText);
}