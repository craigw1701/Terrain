#pragma once

/**
* Stores the vertex data for all the quads on which a text will be rendered.
*/
class TextMeshData 
{
private:
	std::vector<vec2> myVertexPositions;
	std::vector<vec2> myTextureCoords;

public:
	TextMeshData(std::vector<vec2> aVertexPositions, std::vector<vec2> aTextureCoords)
	{
		myVertexPositions = aVertexPositions;
		myTextureCoords = aTextureCoords;
	}

	std::vector<vec2> GetVertexPositions() const
	{
		return myVertexPositions;
	}

	std::vector<vec2> GetTextureCoords() const
	{
		return myTextureCoords;
	}

	int GetVertexCount() const
	{
		return myVertexPositions.size();
	}

};