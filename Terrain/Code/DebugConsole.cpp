#include "DebugConsole.h"

#include "GUIText.h"
#include "GUITexture.h"
#include "FontMetaFile.h"
#include "Loader.h"

bool DebugConsole::ourIsActive = false;
bool DebugConsole::ourRefreshText = false;

FontType* DebugConsole::ourFont = nullptr;
GUIText* DebugConsole::ourCurrentText = nullptr;
GUIText* DebugConsole::ourToolTip = nullptr;
GUITexture* DebugConsole::ourGUITexture = nullptr;

std::vector<GUIText*> DebugConsole::ourHistoryTexts;
std::vector<GUIText*> DebugConsole::ourAutoCompleteTexts;
std::string DebugConsole::ourInputText = "";


int DebugConsole::ourCursorPosition = 0;
int DebugConsole::ourHistoryPosition = -1;
int DebugConsole::ourAutoCompletePosition = -1;

std::vector<std::string> DebugConsole::ourAutoComplete;
std::vector<std::pair<std::string, bool>> DebugConsole::ourHistory;
std::map<std::string, DebugConsole::DebugCommandData> DebugConsole::ourCommands;

void DebugConsole::Setup(Loader& aLoader)
{
	ourFont = new FontType(aLoader.LoadTexture("Fonts/times.png"), "data/Fonts/times.fnt");

	glfwSetCharCallback(GameInfo::ourWindow, &DebugConsole::CharacterInputCallback);
	glfwSetKeyCallback(GameInfo::ourWindow, &DebugConsole::KeyInputCallback);

	ourGUITexture = new GUITexture(aLoader.LoadTexture("DebugConsoleTexture.png"), vec2(0.0f, 0.5f), vec2(1.0f, 0.5f));

	ResetConsole();
}

std::string DebugConsole::GetParam(std::string aCommand, int aParam)
{
	int index = aCommand.find(' ');
	if (index == -1)
		return "";

	std::string temp = aCommand.substr(index, aCommand.find(' ', index + 1));

	return temp;
}

void DebugConsole::CleanUp()
{
	Close();
}

static std::string ToLower(std::string aString)
{
	std::string s;
	s.reserve(aString.size());
	for (auto& c : aString)
		s += tolower(c);

	return s;
}

void DebugConsole::AddCommand(const char* aCommand, const char* aAutoComplete, std::function<std::string(std::string)> aCommandFunc)
{
	DebugCommandData data;
	data.myFullCommand = aCommand;
	data.myAutoCompleteText = aAutoComplete;
	data.myFunc = aCommandFunc;
	ourCommands[ToLower(aCommand)] = data;
}

bool DebugConsole::IsActive() 
{ 
	return ourIsActive; 
}

void DebugConsole::Open() 
{ 
	for (auto& history : ourHistory)
	{
		AddHistory(history.first.c_str(), history.second, true);
	}
	ourIsActive = true; 
}

void DebugConsole::Close()
{
	ourIsActive = false;
	delete ourCurrentText;
	ourCurrentText = nullptr;
	delete ourToolTip;
	ourToolTip = nullptr;
	for (GUIText* text : ourHistoryTexts)
	{
		delete text;
	}
	ourHistoryTexts.clear();
	for (GUIText* text : ourAutoCompleteTexts)
	{
		delete text;
	}
	ourAutoCompleteTexts.clear();
}

void DebugConsole::Toggle()
{
	if (ourIsActive)
		Close();
	else
		Open();
}

void DebugConsole::Update()
{
	if (ourIsActive == false)
		return;

	if (ourRefreshText)
	{
		delete ourCurrentText;
		ourCurrentText = nullptr;
	}

	if (ourCurrentText == nullptr)
	{
		std::string text = "> ";
		if (ourAutoCompletePosition != -1)
		{
			text += ourAutoComplete[ourAutoCompletePosition] + "|";
		}
		else if (ourHistoryPosition != -1)
		{
			text += ourHistory[ourHistoryPosition].first + "|";
		}
		else
		{
			text += ourInputText.substr(0, ourCursorPosition) + "|" + ourInputText.substr(ourCursorPosition);
		}


		float yPos = 0.4f;
		ourCurrentText = new GUIText(text.c_str(), 1, *ourFont, vec2(0.0, yPos), 1.0f, false);
		yPos -= FontMetaFile::LINE_HEIGHT * ourCurrentText->GetFontSize();
		
		for (int i = ourHistoryTexts.size() - 1; i >= 0; i--)
		{				
			yPos -= FontMetaFile::LINE_HEIGHT * ourHistoryTexts.back()->GetFontSize();
			ourHistoryTexts[i]->SetPosition(vec2(0, yPos));
		}
		
		yPos = 0.4f;
		for (int i = 0; i < ourAutoCompleteTexts.size(); i++)
		{
			yPos += FontMetaFile::LINE_HEIGHT * ourCurrentText->GetFontSize();
			ourAutoCompleteTexts[i]->SetPosition(vec2(0, yPos));
		}
	}
}

void DebugConsole::CharacterInputCallback(GLFWwindow* aWindow, unsigned int aChar)
{
	if (ourIsActive == false)
	{
		if (aChar == GLFW_KEY_GRAVE_ACCENT)
		{
			Open();
		}
		return;
	}

	if (aChar >= GLFW_KEY_SPACE && aChar < GLFW_KEY_WORLD_2)
	{
		if (aChar == GLFW_KEY_GRAVE_ACCENT)
		{
			Close();
			return;
		}
		MakeSelection();
		ourInputText = ourInputText.substr(0, ourCursorPosition) + char(aChar) + ourInputText.substr(ourCursorPosition);
		ourCursorPosition++;
		ourRefreshText = true;
	}

	UpdateAutoComplete(ourInputText);
}

void DebugConsole::KeyInputCallback(GLFWwindow* aWindow, int aChar, int scanCode, int anAction, int aMods)
{
	std::string currentText = ourInputText;
	if (anAction == GLFW_RELEASE)
		return;
	
	if (aChar == GLFW_KEY_BACKSPACE)
	{
		currentText = MakeSelection();
		if (aMods == GLFW_MOD_SHIFT)
		{
			ResetConsole();
		}
		else if (ourCursorPosition > 0)
		{
			ourInputText = ourInputText.substr(0, ourCursorPosition-1) + ourInputText.substr(ourCursorPosition);
			ourCursorPosition--;
			ourRefreshText = true;
		}
		currentText = ourInputText;
	}
	else if (aChar == GLFW_KEY_LEFT)
	{
		currentText = MakeSelection();
		ourCursorPosition = ourCursorPosition > 0 ? ourCursorPosition - 1 : 0;
	}
	else if (aChar == GLFW_KEY_RIGHT)
	{
		currentText = MakeSelection();
		ourCursorPosition = ourCursorPosition < ourInputText.size() ? ourCursorPosition + 1 : ourInputText.size();
	}
	else if (aChar == GLFW_KEY_UP)
	{
		if (ourAutoCompleteTexts.size() != 0)
		{
			ourAutoCompletePosition--;
			if (ourAutoCompletePosition < -1)
				ourAutoCompletePosition = ourAutoComplete.size() - 1;

			if(ourAutoCompletePosition != -1)
				currentText = ourAutoComplete[ourAutoCompletePosition];
		}
		else
		{
			int pos = ourHistoryPosition == -1 ? (ourHistory.size() - 1) : ourHistoryPosition - 1;
			for (int i = pos; i >= 0; i--)
			{
				if (!ourHistory[i].second)
				{
					ourHistoryPosition = i;
					ourRefreshText = true;
					currentText = ourHistory[ourHistoryPosition].first;
					UpdateAutoComplete(currentText);
					return;
				}
			}
			ourHistoryPosition = -1;
		}
	}
	else if (aChar == GLFW_KEY_DOWN)
	{
		if (ourAutoCompleteTexts.size() != 0)
		{
			ourAutoCompletePosition++;
			if (ourAutoCompletePosition >= ourAutoComplete.size())
				ourAutoCompletePosition = -1;

			if (ourAutoCompletePosition != -1)
				currentText = ourAutoComplete[ourAutoCompletePosition];
		}	
		else
		{
			for (int i = ourHistoryPosition + 1; i < ourHistory.size(); i++)
			{
				if (!ourHistory[i].second)
				{
					ourHistoryPosition = i;
					ourRefreshText = true;
					currentText = ourHistory[ourHistoryPosition].first;
					UpdateAutoComplete(currentText);
					return;
				}
			}
			ourHistoryPosition = -1;
		}
	}
	else if (aChar == GLFW_KEY_HOME)
	{
		currentText = MakeSelection();
		ourCursorPosition = 0;
	}
	else if (aChar == GLFW_KEY_END)
	{
		currentText = MakeSelection();
		ourCursorPosition = ourInputText.size();
	}
	else if (aChar == GLFW_KEY_ENTER || aChar == GLFW_KEY_KP_ENTER)
	{
		currentText = MakeSelection();
		if (ourInputText.size() != 0)
		{
			std::string reply = "Invalid Command";
			int index = ourInputText.find(' ');
			std::string command = ToLower(ourInputText.substr(0, index));

			auto& iter = ourCommands.find(command);
			if (iter != ourCommands.end())
			{
				reply = ourCommands[command].myFunc(ourInputText.c_str());
			}
			AddHistory(ourInputText.c_str(), false);
			AddHistory(reply.c_str(), true);
			ResetConsole();
		}
	}

	UpdateAutoComplete(ourInputText);
}

void DebugConsole::UpdateAutoComplete(std::string aCurrentText)
{
	for (auto& s : ourAutoCompleteTexts)
	{
		delete s;
	}
	ourAutoComplete.clear();
	ourAutoCompleteTexts.clear();

	if (aCurrentText.size() == 0)
	{
		delete ourToolTip;
		ourToolTip = nullptr;
		return;
	}

	int index = aCurrentText.find(' ');
	std::string command = aCurrentText.substr(0, index);

	DebugCommandData data;
	for (auto& p : ourCommands)
	{
		if (p.first.find(ToLower(command)) != -1)
		{
			data = p.second;
			ourAutoComplete.push_back(p.second.myFullCommand);
		}
	}

	if (ourAutoComplete.size() < 5)
	{
		for (auto& s : ourAutoComplete)
		{
			ourAutoCompleteTexts.push_back(new GUIText(s, 1.0f, *ourFont, vec2(0, 0), 1.0f, false));
		}
	}

	if (ourAutoComplete.size() == 1)
	{
		float y = 0.4f - FontMetaFile::LINE_HEIGHT * ourCurrentText->GetFontSize();
		delete ourToolTip;
		std::string toolTip = data.myFullCommand + " " + data.myAutoCompleteText;
		ourToolTip = new GUIText(toolTip, 1.0f, *ourFont, vec2(0, y), 1.0f, false);
		ourToolTip->SetColour(0, 0.5, 0.5);
		ourRefreshText = true;
	}
	else
	{
		delete ourToolTip;
		ourToolTip = nullptr;
	}
}

std::string DebugConsole::MakeSelection()
{
	if (ourAutoCompletePosition != -1)
	{
		ourInputText = ourAutoComplete[ourAutoCompletePosition] + " ";
		ourCursorPosition = ourInputText.size();
		ourAutoCompletePosition = -1;
	
	}
	if (ourHistoryPosition != -1)
	{
		ourInputText = ourHistory[ourHistoryPosition].first + " ";
		ourCursorPosition = ourInputText.size();
		ourHistoryPosition = -1;
	}
	ourRefreshText = true;
	
	return ourInputText;
}

void DebugConsole::AddHistory(const char* aText, bool aReply, bool aIsReadding)
{
	GUIText* guiText = new GUIText(aText, 1, *ourFont, vec2(0.0, 0), 1.0f, false);
	if (aReply)
		guiText->SetColour(1, 0, 0);
	else
		guiText->SetColour(0, 1, 0);		

	if(!aIsReadding)
		ourHistory.push_back(std::pair<std::string, bool>(aText, aReply));
	ourHistoryTexts.push_back(guiText);
}

void DebugConsole::ResetConsole()
{
	ourInputText = "";
	ourCursorPosition = 0;
	ourHistoryPosition = -1;
	ourAutoCompletePosition = -1;
	ourRefreshText = true;
}