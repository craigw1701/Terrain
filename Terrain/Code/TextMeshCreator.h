#pragma once

#include <vector>

#include "FontMetaFile.h"
#include "GUIText.h"
#include "TextLine.h"
#include "TextMeshData.h"

class TextMeshCreator 
{
private:
	FontMetaFile myMetaData;

public:
	TextMeshCreator(const char* aMetaFile) 
	: myMetaData(aMetaFile)
	{
	}

	TextMeshData CreateTextMesh(GUIText* aGUIText) 
	{
		std::vector<TextLine> lines = CreateStructure(aGUIText);
		TextMeshData data = CreateQuadVertices(aGUIText, lines);
		return data;
	}

	std::vector<TextLine> CreateStructure(GUIText* aGUIText) 
	{
		std::string chars = aGUIText->GetTextString();
		std::vector<TextLine> lines;
		TextLine currentLine(myMetaData.GetSpaceWidth(), aGUIText->GetFontSize(), aGUIText->GetMaxLineSize());
		TextWord currentWord(aGUIText->GetFontSize());
		for (char c : chars) 
		{
			int ascii = (int)c;
			if (ascii == FontMetaFile::SPACE_ASCII) {
				bool added = currentLine.AttemptToAddWord(currentWord);
				if (!added) 
				{
					lines.push_back(currentLine);
					currentLine = TextLine(myMetaData.GetSpaceWidth(), aGUIText->GetFontSize(), aGUIText->GetMaxLineSize());
					currentLine.AttemptToAddWord(currentWord);
				}
				currentWord = TextWord(aGUIText->GetFontSize());
				continue;
			}
			FontCharacter character = myMetaData.GetCharacter(ascii);
			currentWord.AddCharacter(character);
		}
		CompleteStructure(lines, currentLine, currentWord, aGUIText);
		return lines;
	}

	void CompleteStructure(std::vector<TextLine>& lines, TextLine& currentLine, TextWord& currentWord, GUIText* text)
	{
		bool added = currentLine.AttemptToAddWord(currentWord);
		if (!added) 
		{
			lines.push_back(currentLine);
			currentLine = TextLine(myMetaData.GetSpaceWidth(), text->GetFontSize(), text->GetMaxLineSize());
			currentLine.AttemptToAddWord(currentWord);
		}
		lines.push_back(currentLine);
	}

	TextMeshData CreateQuadVertices(GUIText* text, std::vector<TextLine> const& lines) 
	{
		text->SetNumberOfLines(lines.size());
		double curserX = 0.0f;
		double curserY = 0.0f;
		std::vector<vec2> vertices;
		std::vector<vec2> textureCoords;
		for (TextLine line : lines) 
		{
			if (text->IsCentered()) 
			{
				curserX = (line.GetMaxLength() - line.getLineLength()) / 2;
			}
			for (TextWord word : line.GetWords()) 
			{
				for (FontCharacter letter : word.GetCharacters()) 
				{
					AddVerticesForCharacter(curserX, curserY, letter, text->GetFontSize(), vertices);
					AddTexCoords(textureCoords, letter.GetTextureCoordX(), letter.GetTextureCoordY(),
						letter.GetMaxTextureCoordX(), letter.GetMaxTextureCoordY());
					curserX += letter.GetXAdvance() * text->GetFontSize();
				}
				curserX += myMetaData.GetSpaceWidth() * text->GetFontSize();
			}
			curserX = 0;
			curserY += FontMetaFile::LINE_HEIGHT * text->GetFontSize();
		}
		return TextMeshData(vertices, textureCoords);
	}

	void AddVerticesForCharacter(double curserX, double curserY, FontCharacter const& character, double fontSize, std::vector<vec2>& vertices) 
	{
		double x = curserX + (character.GetOffsetX() * fontSize);
		double y = curserY + (character.GetOffsetY() * fontSize);
		double maxX = x + (character.GetSizeX() * fontSize);
		double maxY = y + (character.GetSizeY() * fontSize);
		double properX = (2 * x) - 1;
		double properY = (-2 * y) + 1;
		double properMaxX = (2 * maxX) - 1;
		double properMaxY = (-2 * maxY) + 1;
		AddVertices(vertices, properX, properY, properMaxX, properMaxY);
	}

	static void AddVertices(std::vector<vec2>& vertices, double x, double y, double maxX, double maxY) 
	{
		vertices.push_back(vec2((float)x, (float)y));
		vertices.push_back(vec2((float)x, (float)maxY));
		vertices.push_back(vec2((float)maxX, (float)maxY));
		vertices.push_back(vec2((float)maxX, (float)maxY));
		vertices.push_back(vec2((float)maxX, (float)y));
		vertices.push_back(vec2((float)x, (float)y));
	}

	static void AddTexCoords(std::vector<vec2>& texCoords, double x, double y, double maxX, double maxY) 
	{
		texCoords.push_back(vec2((float)x, (float)y));
		texCoords.push_back(vec2((float)x, (float)maxY));
		texCoords.push_back(vec2((float)maxX, (float)maxY));
		texCoords.push_back(vec2((float)maxX, (float)maxY));
		texCoords.push_back(vec2((float)maxX, (float)y));
		texCoords.push_back(vec2((float)x, (float)y));/*
		texCoords.push_back(vec2((float)0, (float)0));
		texCoords.push_back(vec2((float)0, (float)1));
		texCoords.push_back(vec2((float)1, (float)1));
		texCoords.push_back(vec2((float)1, (float)1));
		texCoords.push_back(vec2((float)1, (float)0));
		texCoords.push_back(vec2((float)0, (float)0));*/
	}
};