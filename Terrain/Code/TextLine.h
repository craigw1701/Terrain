#pragma once

#include <vector>
#include "TextWord.h"

/**
* Represents a line of text during the loading of a text.
*/
class TextLine 
{
private:
	double myMaxLength;
	double mySpaceSize;

	std::vector<TextWord> myWords;
	double myCurrentLineLength = 0;

public:
	/**
	* Creates an empty line.
	*
	* @param aSpaceWidth
	*            - the screen-space width of a space character.
	* @param aFontSize
	*            - the size of font being used.
	* @param aMaxLength
	*            - the screen-space maximum length of a line.
	*/
	TextLine(double aSpaceWidth, double aFontSize, double aMaxLength)
	{
		mySpaceSize = aSpaceWidth * aFontSize;
		myMaxLength = aMaxLength;
	}

	/**
	* Attempt to add a word to the line. If the line can fit the word in
	* without reaching the maximum line length then the word is added and the
	* line length increased.
	*
	* @param word
	*            - the word to try to add.
	* @return {@code true} if the word has successfully been added to the line.
	*/
	bool AttemptToAddWord(TextWord aWord)
	{
		double additionalLength = aWord.GetWordWidth();
		additionalLength += myWords.size() != 0 ? mySpaceSize : 0;
		if (myCurrentLineLength + additionalLength <= myMaxLength) 
		{
			myWords.push_back(aWord);
			myCurrentLineLength += additionalLength;
			return true;
		}
		else 
		{
			return false;
		}
	}

	/**
	* @return The max length of the line.
	*/
	double GetMaxLength() const
	{
		return myMaxLength;
	}

	/**
	* @return The current screen-space length of the line.
	*/
	double getLineLength() const
	{
		return myCurrentLineLength;
	}

	/**
	* @return The list of words in the line.
	*/
	std::vector<TextWord>& GetWords()
	{
		return myWords;
	}

};