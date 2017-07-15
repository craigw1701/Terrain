#include "GUIText.h"

#include <string>

#include "FontType.h"
#include "Maths.h"
#include "TextMaster.h"

GUIText::GUIText(std::string someText, float aFontSize, FontType& aFont, vec2 aPosition, float aMaxLineLength, bool aIsCentered)
	: myTextString(someText)
	, myFontSize(aFontSize)
	, myFont(aFont)
	, myPosition(aPosition)
	, myLineMaxSize(aMaxLineLength)
	, myCenterText(aIsCentered)
{
	TextMaster::LoadText(this);
}

void GUIText::remove()
{
	TextMaster::RemoveText(this);
}

FontType& GUIText::GetFont()
{
	return myFont;
}

void GUIText::SetColour(float r, float g, float b) 
{
	myColour = vec3(r, g, b);
}


vec3 GUIText::GetColour() const
{
	return myColour;
}

int GUIText::GetNumberOfLines() const
{
	return myNumberOfLines;
}

vec2 GUIText::GetPosition() const
{
	return myPosition;
}

int GUIText::GetMesh() const
{
	return myTextMeshVao;
}

void GUIText::SetMeshInfo(int aVAO, int aVerticesCount)
{
	myTextMeshVao = aVAO;
	myVertexCount = aVerticesCount;
}

int GUIText::GetVertexCount() const
{
	return myVertexCount;
}

float GUIText::GetFontSize() const
{
	return myFontSize;
}

void GUIText::SetNumberOfLines(int aNumber)
{
	myNumberOfLines = aNumber;
}

bool GUIText::IsCentered() const
{
	return myCenterText;
}

float GUIText::GetMaxLineSize() const
{
	return myLineMaxSize;
}

std::string GUIText::GetTextString() const
{
	return myTextString;
}