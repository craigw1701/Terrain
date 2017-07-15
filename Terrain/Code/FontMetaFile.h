#pragma once

#include <string>
#include <map>
#include <vector>

#include <sstream>
#include <iostream>
#include <fstream>

#include "FontCharacter.h"
#include "GameInfo.h"

/**
* Provides functionality for getting the values from a font file.
*/
class FontMetaFile 
{
public:
	static constexpr double LINE_HEIGHT = 0.03f;
	static const int SPACE_ASCII = 32;

private:
	static const int PAD_TOP = 0;
	static const int PAD_LEFT = 1;
	static const int PAD_BOTTOM = 2;
	static const int PAD_RIGHT = 3;

	static const int DESIRED_PADDING = 3;


#define SPLITTER ' '
	//static constexpr char* const SPLITTER = " ";
	static constexpr char NUMBER_SEPARATOR = ',';

	double myAspectRatio;

	double myVerticalPerPixelSize;
	double myHorizontalPerPixelSize;
	double mySpaceWidth;
	std::vector<int> myPadding;
	int myPaddingWidth;
	int myPaddingHeight;

	std::map<int, FontCharacter> myMetaData;

	std::ifstream myReader;
	std::map<std::string, std::string> myValues;

public:
	/**
	* Opens a font file in preparation for reading.
	*
	* @param file
	*            - the font file.
	*/
	FontMetaFile(const char* aFile) 
		:myReader(aFile)
	{
		myAspectRatio = (double)GameInfo::ourScreenWidth / (double)GameInfo::ourScreenHeight;
		OpenFile(aFile);
		LoadPaddingData();
		LoadLineSizes();
		int imageWidth = GetValueOfVariable("scaleW");
		LoadCharacterData(imageWidth);
		Close();
	}

	double GetSpaceWidth() const
	{
		return mySpaceWidth;
	}

	FontCharacter GetCharacter(int aASCII) const
	{
		return myMetaData.find(aASCII)->second;
	}

	/**
	* Read in the next line and store the variable values.
	*
	* @return {@code true} if the end of the file hasn't been reached.
	*/
	bool ProcessNextLine() 
	{
		myValues.clear();
		std::string line;
		std::getline(myReader, line);

		if (line.size() == 0)
		{
			return false;
		}

		std::string s;
		std::vector<std::string> p;
		std::istringstream valueString(line);
		while (std::getline(valueString, s, SPLITTER))
		{
			std::string s2;
			std::istringstream valueString2(s);
			while (std::getline(valueString2, s2, '='))
			{
				if(s2.size() != s.size())
					p.push_back(s2);
			}
		}
		for (int i = 0; i < p.size(); i += 2)
		{
			myValues.insert(std::pair<std::string, std::string>(p[i], p[i+1]));
		}
		return true;
	}

	/**
	* Gets the {@code int} value of the variable with a certain name on the
	* current line.
	*
	* @param variable
	*            - the name of the variable.
	* @return The value of the variable.
	*/
	int GetValueOfVariable(std::string aVariable) 
	{
		auto iter = myValues.find(aVariable);
		if(iter != myValues.end())
			return atoi(iter->second.c_str());

		assert(false);
		return -1;
	}

	/**
	* Gets the array of ints associated with a variable on the current line.
	*
	* @param variable
	*            - the name of the variable.
	* @return The int array of values associated with the variable.
	*/
	std::vector<int> GetValuesOfVariable(std::string aVariable)
	{
		std::istringstream valueString(myValues.find(aVariable)->second);
		std::vector<int> actualValues;
		string s;
		while (std::getline(valueString, s, NUMBER_SEPARATOR))
		{
			actualValues.push_back(atoi(s.c_str()));
		}

		return actualValues;
	}

	/**
	* Closes the font file after finishing reading.
	*/
	void Close()
	{
		myReader.close();
	}

	/**
	* Opens the font file, ready for reading.
	*
	* @param file
	*            - the font file.
	*/
	void OpenFile(const char* aFile)
	{
		//myReader.open(aFile);
		if (!myReader)
		{
			printf("ERROR");
			return;
		}
	}

	/**
	* Loads the data about how much padding is used around each character in
	* the texture atlas.
	*/
	void LoadPaddingData() 
	{
		ProcessNextLine();
		myPadding = GetValuesOfVariable("padding");
		myPaddingWidth = myPadding[PAD_LEFT] + myPadding[PAD_RIGHT];
		myPaddingHeight = myPadding[PAD_TOP] + myPadding[PAD_BOTTOM];
	}

	/**
	* Loads information about the line height for this font in pixels, and uses
	* this as a way to find the conversion rate between pixels in the texture
	* atlas and screen-space.
	*/
	void LoadLineSizes()
	{
		ProcessNextLine();
		int lineHeightPixels = GetValueOfVariable("lineHeight") - myPaddingHeight;
		myVerticalPerPixelSize = LINE_HEIGHT / (double)lineHeightPixels;
		myHorizontalPerPixelSize = myVerticalPerPixelSize / myAspectRatio;
	}

	/**
	* Loads in data about each character and stores the data in the
	* {@link Character} class.
	*
	* @param imageWidth
	*            - the width of the texture atlas in pixels.
	*/
	void LoadCharacterData(int aImageWidth) 
	{
		ProcessNextLine();
		ProcessNextLine();
		while (ProcessNextLine()) 
		{
			FontCharacter c = LoadCharacter(aImageWidth);
			if (c.GetId() != -1)
			{
				myMetaData.insert(std::pair<int, FontCharacter>(c.GetId(), c));
			}
		}
	}

	/**
	* Loads all the data about one character in the texture atlas and converts
	* it all from 'pixels' to 'screen-space' before storing. The effects of
	* padding are also removed from the data.
	*
	* @param imageSize
	*            - the size of the texture atlas in pixels.
	* @return The data about the character.
	*/
	FontCharacter LoadCharacter(int anImageSize) 
	{
		int id = GetValueOfVariable("id");
		if (id == SPACE_ASCII) 
		{
			mySpaceWidth = (GetValueOfVariable("xadvance") - myPaddingWidth) * myHorizontalPerPixelSize;
			return FontCharacter();
		}
		double xTex = ((double)GetValueOfVariable("x") + (myPadding[PAD_LEFT] - DESIRED_PADDING)) / anImageSize;
		double yTex = ((double)GetValueOfVariable("y") + (myPadding[PAD_TOP] - DESIRED_PADDING)) / anImageSize;
		int width = GetValueOfVariable("width") - (myPaddingWidth - (2 * DESIRED_PADDING));
		int height = GetValueOfVariable("height") - ((myPaddingHeight)-(2 * DESIRED_PADDING));
		double quadWidth = width * myHorizontalPerPixelSize;
		double quadHeight = height * myVerticalPerPixelSize;
		double xTexSize = (double)width / anImageSize;
		double yTexSize = (double)height / anImageSize;
		double xOff = (GetValueOfVariable("xoffset") + myPadding[PAD_LEFT] - DESIRED_PADDING) * myHorizontalPerPixelSize;
		double yOff = (GetValueOfVariable("yoffset") + (myPadding[PAD_TOP] - DESIRED_PADDING)) * myVerticalPerPixelSize;
		double xAdvance = (GetValueOfVariable("xadvance") - myPaddingWidth) * myHorizontalPerPixelSize;

		return FontCharacter(id, xTex, yTex, xTexSize, yTexSize, xOff, yOff, quadWidth, quadHeight, xAdvance);
	}
};