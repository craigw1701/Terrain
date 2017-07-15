#pragma once

#include <vector>
#include "FontCharacter.h"
/**
* During the loading of a text this represents one word in the text.
*
*/
class TextWord 
{
private:
	std::vector<FontCharacter> myCharacters;
	double myWidth = 0;
	double myFontSize;

public:
	/**
	* Create a new empty word.
	* @param fontSize - the font size of the text which this word is in.
	*/
	TextWord(double aFontSize) {
		myFontSize = aFontSize;
	}

	/**
	* Adds a character to the end of the current word and increases the screen-space width of the word.
	* @param character - the character to be added.
	*/
	void AddCharacter(FontCharacter aCharacter)
	{
		myCharacters.push_back(aCharacter);
		myWidth += aCharacter.GetXAdvance() * myFontSize;
	}

	/**
	* @return The list of characters in the word.
	*/
	std::vector<FontCharacter> GetCharacters() const
	{
		return myCharacters;
	}

	/**
	* @return The width of the word in terms of screen size.
	*/
	double GetWordWidth() const
	{
		return myWidth;
	}
};