#include "DebugConsole.h"

#include "GUIText.h"
#include "GUITexture.h"
#include "FontMetaFile.h"
#include "Loader.h"
#include "StringUtils.h"

unique_ptr<DebugConsole> DebugConsole::ourInstance = nullptr;

DebugConsole::DebugConsole() 
: myCurrentText(nullptr)
, myToolTip(nullptr)
, myFont(nullptr)
{

}

DebugConsole::~DebugConsole()
{

}

void DebugConsole::AddVariable(void* aVar, DebugConsoleVarType aType, const char* aName)
{
	// TODO:CW Check main thread?
	std::string nameLower = ToLower(aName);

	myVariables[nameLower] = std::make_unique<DebugConsoleVar>(aName, aVar, aType, myVariableIndex++);
}

void DebugConsole::Setup(Loader& aLoader)
{
	if (ourInstance)
		return;

	ourInstance = std::unique_ptr<DebugConsole>(new DebugConsole());
	ourInstance->myFont = std::make_unique<FontType>(aLoader.LoadTexture("Fonts/times.png"), "data/Fonts/times.fnt");
	ourInstance->myGUITexture = make_unique<GUITexture>(aLoader.LoadTexture("DebugConsoleTexture.png"), vec2(0.0f, 0.5f), vec2(1.0f, 0.5f));

	glfwSetCharCallback(GameInfo::ourWindow, &DebugConsole::CharacterInputCallback);
	glfwSetKeyCallback(GameInfo::ourWindow, &DebugConsole::KeyInputCallback);

	ourInstance->ResetConsole();
}

std::string DebugConsole::GetParam(std::string aCommand, int aParam) const
{
	//TODO:CW have this return the "aParm'th" parmeter
	return aCommand.substr(0, aCommand.find(' '));
}

void DebugConsole::CleanUp()
{
	if (ourInstance)
	{
		ourInstance->myVariables.clear();
		ourInstance->myCommands.clear();
		ourInstance->myGUITexture = nullptr;
		ourInstance->Close();
		ourInstance = nullptr;
	}
}

void DebugConsole::AddCommand(const char* aCommand, const char* aAutoComplete, std::function<std::string(std::string)> aCommandFunc)
{
	DebugCommandData data;
	data.myFullCommand = aCommand;
	data.myAutoCompleteText = aAutoComplete;
	data.myFunc = aCommandFunc;
	myCommands[ToLower(aCommand)] = data;
}

bool DebugConsole::IsActive() 
{ 
	return ourInstance ? ourInstance->myIsActive : false;
}

void DebugConsole::Open() 
{ 
	for (auto& history : myHistory)
	{
		AddHistory(history.first.c_str(), history.second, true);
	}
	myIsActive = true; 
}

void DebugConsole::Close()
{
	myIsActive = false;
	myCurrentText = nullptr;
	myToolTip = nullptr;
	myHistoryTexts.clear();
	myAutoCompleteTexts.clear();
}

void DebugConsole::Toggle()
{
	if (myIsActive)
		Close();
	else
		Open();
}

void DebugConsole::Update()
{
	if (myIsActive == false)
		return;

	if (myRefreshText)
	{
		myCurrentText = nullptr;
	}

	if (myCurrentText == nullptr)
	{
		std::string text = "> ";
		if (myAutoCompletePosition != -1)
		{
			text += myAutoComplete[myAutoCompletePosition] + "|";
		}
		else if (myHistoryPosition != -1)
		{
			text += myHistory[myHistoryPosition].first + "|";
		}
		else
		{
			text += myInputText.substr(0, myCursorPosition) + "|" + myInputText.substr(myCursorPosition);
		}


		float yPos = 0.4f;
		myCurrentText = std::make_unique<GUIText>(text.c_str(), 1.0f, *myFont, vec2(0.0, yPos), 1.0f, false);
		yPos -= FontMetaFile::LINE_HEIGHT * myCurrentText->GetFontSize();
		
		for (int i = myHistoryTexts.size() - 1; i >= 0; i--)
		{				
			yPos -= FontMetaFile::LINE_HEIGHT * myHistoryTexts.back()->GetFontSize();
			myHistoryTexts[i]->SetPosition(vec2(0, yPos));
		}
		
		yPos = 0.4f;
		for (unsigned int i = 0; i < myAutoCompleteTexts.size(); i++)
		{
			yPos += FontMetaFile::LINE_HEIGHT * myCurrentText->GetFontSize();
			myAutoCompleteTexts[i]->SetPosition(vec2(0, yPos));
		}
	}
}


void DebugConsole::CharacterInputCallback(GLFWwindow* aWindow, unsigned int aChar)
{
	if (ourInstance == nullptr)
		return;
	ourInstance->CharacterInputCallback_Internal(aWindow, aChar);
}

void DebugConsole::CharacterInputCallback_Internal(GLFWwindow* aWindow, unsigned int aChar)
{
	if(ourInstance == nullptr)
		return;
	if (myIsActive == false)
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
		myInputText = myInputText.substr(0, myCursorPosition) + char(aChar) + myInputText.substr(myCursorPosition);
		myCursorPosition++;
		myRefreshText = true;
	}

	UpdateAutoComplete(myInputText);
}

void DebugConsole::KeyInputCallback(GLFWwindow* aWindow, int aChar, int scanCode, int anAction, int aMods)
{
	if (ourInstance == nullptr)
		return;

	ourInstance->KeyInputCallback_Internal(aWindow, aChar, scanCode, anAction, aMods);
}

void DebugConsole::KeyInputCallback_Internal(GLFWwindow* aWindow, int aChar, int scanCode, int anAction, int aMods)
{
	std::string currentText = myInputText;
	if (anAction == GLFW_RELEASE)
		return;
	
	if (aChar == GLFW_KEY_BACKSPACE)
	{
		currentText = MakeSelection();
		if (aMods == GLFW_MOD_SHIFT)
		{
			ResetConsole();
		}
		else if (myCursorPosition > 0)
		{
			myInputText = myInputText.substr(0, myCursorPosition-1) + myInputText.substr(myCursorPosition);
			myCursorPosition--;
			myRefreshText = true;
		}
		currentText = myInputText;
	}
	else if (aChar == GLFW_KEY_LEFT)
	{
		currentText = MakeSelection();
		myCursorPosition = myCursorPosition > 0 ? myCursorPosition - 1 : 0;
	}
	else if (aChar == GLFW_KEY_RIGHT)
	{
		currentText = MakeSelection();
		myCursorPosition = static_cast<unsigned int>(myCursorPosition) < myInputText.size() ? myCursorPosition + 1 : myInputText.size();
	}
	else if (aChar == GLFW_KEY_UP)
	{
		if (myAutoCompleteTexts.size() != 0)
		{
			myAutoCompletePosition--;
			if (myAutoCompletePosition < -1)
				myAutoCompletePosition = myAutoComplete.size() - 1;

			if(myAutoCompletePosition != -1)
				currentText = myAutoComplete[myAutoCompletePosition];
		}
		else
		{
			int pos = myHistoryPosition == -1 ? (myHistory.size() - 1) : myHistoryPosition - 1;
			for (int i = pos; i >= 0; i--)
			{
				if (!myHistory[i].second)
				{
					myHistoryPosition = i;
					myRefreshText = true;
					currentText = myHistory[myHistoryPosition].first;
					UpdateAutoComplete(currentText);
					return;
				}
			}
			myHistoryPosition = -1;
		}
	}
	else if (aChar == GLFW_KEY_DOWN)
	{
		if (myAutoCompleteTexts.size() != 0)
		{
			myAutoCompletePosition++;
			if (static_cast<unsigned int>(myAutoCompletePosition) >= myAutoComplete.size())
				myAutoCompletePosition = -1;

			if (myAutoCompletePosition != -1)
				currentText = myAutoComplete[myAutoCompletePosition];
		}	
		else
		{
			for (unsigned int i = myHistoryPosition + 1; i < myHistory.size(); i++)
			{
				if (!myHistory[i].second)
				{
					myHistoryPosition = i;
					myRefreshText = true;
					currentText = myHistory[myHistoryPosition].first;
					UpdateAutoComplete(currentText);
					return;
				}
			}
			myHistoryPosition = -1;
		}
	}
	else if (aChar == GLFW_KEY_HOME)
	{
		currentText = MakeSelection();
		myCursorPosition = 0;
	}
	else if (aChar == GLFW_KEY_END)
	{
		currentText = MakeSelection();
		myCursorPosition = myInputText.size();
	}
	else if (aChar == GLFW_KEY_ENTER || aChar == GLFW_KEY_KP_ENTER)
	{
		currentText = MakeSelection();
		if (myInputText.size() != 0)
		{
			std::string reply = "Invalid Command";
			int index = myInputText.find(' ');
			std::string command = ToLower(myInputText.substr(0, index));

			auto& iter = myCommands.find(command);
			if (iter != myCommands.end())
			{
				std::string parameters = TrimWhitespace(myInputText.substr(command.length()));
				reply = myCommands[command].myFunc(parameters);
			}
			else
			{
				auto& variableIter = myVariables.find(command);
				if (variableIter != myVariables.end())
				{
					std::string parameters = TrimWhitespace(myInputText.substr(command.length()));
					if (parameters.size() != 0)
						myVariables[command]->Set(parameters.c_str());

					myVariables[command]->Get(reply);
				}
			}
			AddHistory(myInputText.c_str(), false);
			AddHistory(reply.c_str(), true);
			ResetConsole();
		}
	}

	UpdateAutoComplete(myInputText);
}

void DebugConsole::UpdateAutoComplete(std::string aCurrentText)
{
	myAutoComplete.clear();
	myAutoCompleteTexts.clear();

	if (aCurrentText.size() == 0)
	{
		myToolTip = nullptr;
		return;
	}

	int index = aCurrentText.find(' ');
	std::string command = aCurrentText.substr(0, index);

	DebugCommandData data;
	for (auto& p : myCommands)
	{
		if (p.first.find(ToLower(command)) != -1)
		{
			data = p.second;
			myAutoComplete.push_back(p.second.myFullCommand);
		}
	}
	
	DebugConsoleVar* debugVar = nullptr;
	for (auto& var : myVariables)
	{
		if (var.first.find(ToLower(command)) != -1)
		{
			debugVar = var.second.get();
			myAutoComplete.push_back(var.second->GetName());
		}
	}

	if (myAutoComplete.size() < 5)
	{
		for (auto& s : myAutoComplete)
		{
			myAutoCompleteTexts.push_back(std::make_unique<GUIText>(s, 1.0f, *myFont, vec2(0, 0), 1.0f, false));
		}
	}

	if (myAutoComplete.size() == 1 && debugVar == nullptr) // TODO:CW Support tooltips on variables
	{
		float y = 0.4f - FontMetaFile::LINE_HEIGHT * myCurrentText->GetFontSize();
		myToolTip = nullptr;
		std::string toolTip = data.myFullCommand + " " + data.myAutoCompleteText;
		myToolTip = std::make_unique<GUIText>(toolTip, 1.0f, *myFont, vec2(0, y), 1.0f, false);
		myToolTip->SetColour(0, 0.5, 0.5);
		myRefreshText = true;
	}
	else
	{
		myToolTip = nullptr;
	}
}

std::string DebugConsole::MakeSelection()
{
	if (myAutoCompletePosition != -1)
	{
		myInputText = myAutoComplete[myAutoCompletePosition] + " ";
		myCursorPosition = myInputText.size();
		myAutoCompletePosition = -1;
	
	}
	if (myHistoryPosition != -1)
	{
		myInputText = myHistory[myHistoryPosition].first + " ";
		myCursorPosition = myInputText.size();
		myHistoryPosition = -1;
	}
	myRefreshText = true;
	
	return myInputText;
}

void DebugConsole::AddHistory(const char* aText, bool aReply, bool aIsReadding)
{
	unique_ptr<GUIText> guiText = std::make_unique<GUIText>(aText, 1.0f, *myFont, vec2(0.0, 0), 1.0f, false);
	if (aReply)
		guiText->SetColour(0.6f, 0.6f, 0.6f);
	else
		guiText->SetColour(0.8f, 0.8f, 0.8f);		

	if(!aIsReadding)
		myHistory.push_back(std::pair<std::string, bool>(aText, aReply));
	myHistoryTexts.push_back(std::move(guiText));
}

void DebugConsole::ResetConsole()
{
	myInputText = "";
	myCursorPosition = 0;
	myHistoryPosition = -1;
	myAutoCompletePosition = -1;
	myRefreshText = true;
}