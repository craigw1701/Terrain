#pragma once

#include <map>
#include <vector>
#include <functional>

class FontType;
class GUIText;
class GUITexture;
struct GLFWwindow;
class Loader;

class DebugConsole
{
public:
	//typedef DebugConsoleCommandFunc std::function<const char*(std::string)>;

	static void Setup(Loader& aLoader);
	static void CleanUp();
	static bool IsActive();
	static void Open();
	static void Close();
	static void Toggle();

	static void Update();
	static void AddCommand(const char* aCommand, const char* aAutoComplete, std::function<std::string(std::string)> aCommandFunc);
	static std::string GetParam(std::string aCommand, int aParam);

	static GUITexture* ourGUITexture;
private:
	struct DebugCommandData
	{
		std::string myFullCommand;
		std::string myAutoCompleteText;
		std::function<std::string(std::string)> myFunc;
		//DebugConsole::DebugConsoleCommandFunc myFunc;
	};

	static bool ourIsActive;
	static GUIText* ourCurrentText;
	static GUIText* ourToolTip;
	static FontType* ourFont;
	static std::vector<GUIText*> ourHistoryTexts;
	static std::vector<GUIText*> ourAutoCompleteTexts;
	static std::string ourInputText;
	static std::vector<std::string> ourAutoComplete;
	static std::vector<std::pair<std::string, bool>> ourHistory;
	static std::map<std::string, DebugCommandData> ourCommands;
	static bool ourRefreshText;
	static int ourCursorPosition;
	static int ourHistoryPosition;
	static int ourAutoCompletePosition;

	static void CharacterInputCallback(GLFWwindow* aWindow, unsigned int aChar);
	static void AddHistory(const char* aText, bool aReply, bool aIsReadding = false);
	static void KeyInputCallback(GLFWwindow* aWindow, int aChar, int scanCode, int anAction, int aMods);
	static void UpdateAutoComplete(std::string aCurrentText);
	static void ResetConsole();	
	static std::string MakeSelection();
};