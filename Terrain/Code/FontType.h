#pragma once

class TextMeshCreator;
class GUIText;
class TextMeshData;
/**
* Represents a font. It holds the font's texture atlas as well as having the
* ability to create the quad vertices for any text using this font.
*
*/
class FontType 
{
private:
	int myTextureAtlas;
	TextMeshCreator* myLoader;

public:
	/**
	* Creates a new font and loads up the data about each character from the
	* font file.
	*
	* @param aTextureAtlas
	*            - the ID of the font atlas texture.
	* @param aFontFile
	*            - the font file containing information about each character in
	*            the texture atlas.
	*/
	FontType(int aTextureAtlas, const char* aFontFile);
	~FontType();

	bool operator< (const FontType& rhs) const
	{
		return myTextureAtlas < rhs.myTextureAtlas;
	}
	/**
	* @return The font texture atlas.
	*/
	int GetTextureAtlas() const;

	/**
	* Takes in an unloaded text and calculate all of the vertices for the quads
	* on which this text will be rendered. The vertex positions and texture
	* coords and calculated based on the information from the font file.
	*
	* @param aText
	*            - the unloaded text.
	* @return Information about the vertices of all the quads.
	*/
	TextMeshData LoadText(GUIText* aText);

};