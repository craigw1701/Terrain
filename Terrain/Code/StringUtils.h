#pragma once

#include <string>

static std::string TrimWhitespace(std::string aString)
{
	std::string s = aString;
	while (s.find(' ') == 0)
	{
		s = s.substr(1);
	}

	while (s.rfind(' ') == s.size() - 1)
	{
		s = s.substr(0, s.size() - 1);

		if (s.size() == 0)
			return s;
	}
	return s;
}

static std::string ToLower(std::string aString)
{
	std::string s;
	s.reserve(aString.size());
	for (auto& c : aString)
		s += tolower(c);

	return s;
}