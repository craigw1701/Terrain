#pragma once

#include "GL.h"

#include "lodepng.h"

#include <vector>

class TextureData
{
public:
	std::vector<unsigned char> myImageBuffer;
	unsigned int myWidth;
	unsigned int myHeight;
};

TextureData GetTextureData(const char * imagepath);


// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

GLuint DecodeOneStep(const char* filename, float aLODBias);

