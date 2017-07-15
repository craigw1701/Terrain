#pragma once

class FontCharacter 
{
private:
	int myId;
	double myTextureCoordX;
	double myTextureCoordY;
	double myMaxTextureCoordX;
	double myMaxTextureCoordY;
	double myOffsetX;
	double myOffsetY;
	double mySizeX;
	double mySizeY;
	double myXAdvance;

	/**
	* @param anId
	*            - the ASCII value of the character.
	* @param aTextureCoordX
	*            - the x texture coordinate for the top left corner of the
	*            character in the texture atlas.
	* @param aTextureCoordY
	*            - the y texture coordinate for the top left corner of the
	*            character in the texture atlas.
	* @param aTexSizeX
	*            - the width of the character in the texture atlas.
	* @param aTexSizeY
	*            - the height of the character in the texture atlas.
	* @param aOffsetX
	*            - the x distance from the curser to the left edge of the
	*            character's quad.
	* @param aOffsetY
	*            - the y distance from the curser to the top edge of the
	*            character's quad.
	* @param aSizeX
	*            - the width of the character's quad in screen space.
	* @param aSizeY
	*            - the height of the character's quad in screen space.
	* @param aXAdvance
	*            - how far in pixels the cursor should advance after adding
	*            this character.
	*/
public:
	FontCharacter()
		:FontCharacter(-1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
	{

	}
	FontCharacter(int anId, double aTextureCoordX, double aTextureCoordY, double aTexSizeX, double aTexSizeY,
		double aOffsetX, double aOffsetY, double aSizeX, double aSizeY, double aXAdvance) 
	{
		myId = anId;
		myTextureCoordX = aTextureCoordX;
		myTextureCoordY = aTextureCoordY;
		myOffsetX = aOffsetX;
		myOffsetY = aOffsetY;
		mySizeX = aSizeX;
		mySizeY = aSizeY;
		myMaxTextureCoordX = aTexSizeX + aTextureCoordX;
		myMaxTextureCoordY = aTexSizeY + aTextureCoordY;
		myXAdvance = aXAdvance;
	}

	int GetId()
	{
		return myId;
	}

	double GetTextureCoordX() const
	{
		return myTextureCoordX;
	}

	double GetTextureCoordY() const
	{
		return myTextureCoordY;
	}

	double GetMaxTextureCoordX() const
	{
		return myMaxTextureCoordX;
	}

	double GetMaxTextureCoordY() const
	{
		return myMaxTextureCoordY;
	}

	double GetOffsetX() const
	{
		return myOffsetX;
	}

	double GetOffsetY() const
	{
		return myOffsetY;
	}

	double GetSizeX() const
	{
		return mySizeX;
	}

	double GetSizeY() const
	{
		return mySizeY;
	}

	double GetXAdvance() const
	{
		return myXAdvance;
	}
};