#pragma once

#include <map>
#include <vector>
#include <functional>
#include <string>
#include <memory>
#include "GL.h"

class FontType;
class GUIText;
class GUITexture;
struct GLFWwindow;
class Loader;

using std::unique_ptr;
using std::vector;
using std::map;


enum class DebugConsoleVarType
{
	BOOL,
	INT,
	FLOAT,
	VEC3,
};

class DebugConsoleVar
{
	void* myVariablePtr;
	std::string myName;
	DebugConsoleVarType myType;
	int myIndex;

	std::string BuildReply(std::string const& aReply);
public:
	DebugConsoleVar() {}
	DebugConsoleVar(const char* aName, void* aVariable, DebugConsoleVarType aVarType, int aIndex)
		: myName(aName)
		, myVariablePtr(aVariable)
		, myType(aVarType)
		, myIndex(aIndex)
	{
		//DebugConsole::Register(this);
	}

	const char* GetName() const;

	bool Set(std::vector<std::string> someParams);
	bool Get(std::string& aReturnedString);

	// TODO:CW add callback
	// TODO:CW Support ranges
};

class DebugConsole
{
	DebugConsole();
	
	static unique_ptr<DebugConsole> ourInstance;
public:
	~DebugConsole();

	using DebugConsoleCommandFunc = std::string(std::vector<std::string>);
	static DebugConsole* GetInstance() { return ourInstance.get(); }
	static void Setup(Loader& aLoader);
	static void CleanUp();

	static bool IsActive();
	void Open();
	void Close();
	void Toggle();

	void Update();
	void AddCommand(const char* aCommand, const char* aAutoComplete, std::function<DebugConsoleCommandFunc> aCommandFunc);
	//static void Register(, const char* aAutoComplete, std::function<std::string(std::string)> aCommandFunc);
	std::string GetParam(std::string aCommand, int aParam) const;

	unique_ptr<GUITexture> myGUITexture;

	void AddVariable(const char* aName, bool& aBool) { AddVariable(&aBool, DebugConsoleVarType::BOOL, aName); }
	void AddVariable(const char* aName, int& anInt) { AddVariable(&anInt, DebugConsoleVarType::INT, aName); }
	void AddVariable(const char* aName, float& aFloat) { AddVariable(&aFloat, DebugConsoleVarType::FLOAT, aName); }
	void AddVariable(const char* aName, vec3& aVec3) { AddVariable(&aVec3, DebugConsoleVarType::VEC3, aName); }
	
private:
	struct DebugCommandData
	{
		std::string myFullCommand;
		std::string myAutoCompleteText;
		std::function<std::string(std::vector<std::string>)> myFunc;
		//DebugConsole::DebugConsoleCommandFunc myFunc;
	};
	void AddVariable(void* aVar, DebugConsoleVarType aType, const char* aName);

	bool myIsActive = false;
	unique_ptr<GUIText> myCurrentText;
	unique_ptr<GUIText> myToolTip;
	unique_ptr<FontType> myFont;
	vector<unique_ptr<GUIText>> myHistoryTexts;
	vector<unique_ptr<GUIText>> myAutoCompleteTexts;
	std::string myInputText;
	vector<std::string> myAutoComplete;
	vector<std::pair<std::string, bool>> myHistory;
	map<std::string, DebugCommandData> myCommands;
	map<std::string, unique_ptr<DebugConsoleVar>> myVariables;
	bool myRefreshText = false;
	int myCursorPosition = 0;
	int myHistoryPosition = -1;
	int myAutoCompletePosition = -1;

	static void CharacterInputCallback(GLFWwindow* aWindow, unsigned int aChar);
	static void KeyInputCallback(GLFWwindow* aWindow, int aChar, int scanCode, int anAction, int aMods);
	void CharacterInputCallback_Internal(GLFWwindow* aWindow, unsigned int aChar);
	void KeyInputCallback_Internal(GLFWwindow* aWindow, int aChar, int scanCode, int anAction, int aMods);

	void AddHistory(const char* aText, bool aReply, bool aIsReadding = false);
	void UpdateAutoComplete(std::string aCurrentText);
	void ResetConsole();	
	std::string MakeSelection();

	int myVariableIndex = 0;
};