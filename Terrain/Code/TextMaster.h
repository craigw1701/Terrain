#pragma once

#include <map>
#include <vector>

//#include ".h"
#include "FontType.h"

class GUIText;
class Loader;
class FontRenderer;

class TextMaster
{
private:
	static Loader* ourLoader;
	static FontRenderer* ourRenderer;
	static std::map<FontType, std::vector<GUIText*>> ourTexts;
public:
	static void Init(Loader* aLoader);

	static void Render();

	static void LoadText(GUIText* someText);

	static void RemoveText(GUIText* aGUIText);

	static void CleanUp();
};
