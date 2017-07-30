#pragma once

#include <string>

static std::string TrimWhitespace(std::string aString)
{
	std::string s = aString;
	int startIndex = 0;
	int endIndex = aString.size();
	for (int i = 0; i < endIndex; i++)
	{
		if (aString[i] == ' ' || aString[i] == '=')
			startIndex = i + 1;
		else
			break;
	}
	
	for (int i = endIndex; i >= startIndex; i--)
	{
		if (aString[i] == ' ' || aString[i] == '=')
			endIndex = i;
		else
			break;
	}	

	return aString.substr(startIndex, endIndex - startIndex);

}

static std::string ToLower(std::string aString)
{
	std::string s;
	s.reserve(aString.size());
	for (auto& c : aString)
		s += tolower(c);

	return s;
}