#pragma once

#include <string>

#include "FontType.h"
#include "Maths.h"

/**
* Represents a piece of text in the game.
*/

class GUIText 
{
private:

	std::string myTextString;
	float myFontSize;

	int myTextMeshVao;
	int myVertexCount;
	vec3 myColour = vec3(0.0f, 0.0f, 0.0f);

	vec2 myPosition;
	float myLineMaxSize;
	int myNumberOfLines;

	FontType myFont;

	bool myCenterText = false;

public:
	/**
	* Creates a new text, loads the text's quads into a VAO, and adds the text
	* to the screen.
	*
	* @param someText
	*            - the text.
	* @param aFontSize
	*            - the font size of the text, where a font size of 1 is the
	*            default size.
	* @param aFont
	*            - the font that this text should use.
	* @param aPosition
	*            - the position on the screen where the top left corner of the
	*            text should be rendered. The top left corner of the screen is
	*            (0, 0) and the bottom right is (1, 1).
	* @param aMaxLineLength
	*            - basically the width of the virtual page in terms of screen
	*            width (1 is full screen width, 0.5 is half the width of the
	*            screen, etc.) Text cannot go off the edge of the page, so if
	*            the text is longer than this length it will go onto the next
	*            line. When text is centered it is centered into the middle of
	*            the line, based on this line length value.
	* @param aIsCentered
	*            - whether the text should be centered or not.
	*/
	GUIText(std::string someText, float aFontSize, FontType& aFont, vec2 aPosition, float aMaxLineLength, bool aIsCentered);

	friend bool operator==(GUIText const& aLHS, GUIText const& aRHS)
	{
		return aLHS.myTextString == aRHS.myTextString &&
			aLHS.myFontSize == aRHS.myFontSize &&
			aLHS.myTextMeshVao == aRHS.myTextMeshVao &&
			aLHS.myVertexCount == aRHS.myVertexCount &&
			aLHS.myColour == aRHS.myColour &&
			aLHS.myPosition == aRHS.myPosition &&
			aLHS.myLineMaxSize == aRHS.myLineMaxSize &&
			aLHS.myNumberOfLines == aRHS.myNumberOfLines &&
			//aLHS.myFont == aRHS.myFont && // TODO:CW
			aLHS.myCenterText == aRHS.myCenterText;
	}
	/**
	* Remove the text from the screen.
	*/
	void remove();

	/**
	* @return The font used by this text.
	*/
	FontType& GetFont();

	/**
	* Set the colour of the text.
	*
	* @param r
	*            - red value, between 0 and 1.
	* @param g
	*            - green value, between 0 and 1.
	* @param b
	*            - blue value, between 0 and 1.
	*/
	void SetColour(float r, float g, float b);

	/**
	* @return the colour of the text.
	*/
	vec3 GetColour() const;

	/**
	* @return The number of lines of text. This is determined when the text is
	*         loaded, based on the length of the text and the max line length
	*         that is set.
	*/
	int GetNumberOfLines() const;

	/**
	* @return The position of the top-left corner of the text in screen-space.
	*         (0, 0) is the top left corner of the screen, (1, 1) is the bottom
	*         right.
	*/
	vec2 GetPosition() const;

	/**
	* @return the ID of the text's VAO, which contains all the vertex data for
	*         the quads on which the text will be rendered.
	*/
	int GetMesh() const;

	/**
	* Set the VAO and vertex count for this text.
	*
	* @param vao
	*            - the VAO containing all the vertex data for the quads on
	*            which the text will be rendered.
	* @param verticesCount
	*            - the total number of vertices in all of the quads.
	*/
	void SetMeshInfo(int aVAO, int aVerticesCount);

	/**
	* @return The total number of vertices of all the text's quads.
	*/
	int GetVertexCount() const;

	/**
	* @return the font size of the text (a font size of 1 is normal).
	*/
	float GetFontSize() const;

	/**
	* Sets the number of lines that this text covers (method used only in
	* loading).
	*
	* @param number
	*/
	void SetNumberOfLines(int aNumber);

	/**
	* @return {@code true} if the text should be centered.
	*/
	bool IsCentered() const;

	/**
	* @return The maximum length of a line of this text.
	*/
	float GetMaxLineSize() const;

	/**
	* @return The string of text.
	*/
	std::string GetTextString() const;
};