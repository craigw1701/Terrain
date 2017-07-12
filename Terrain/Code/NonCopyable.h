#pragma once

class NonCopyable
{
public:
	NonCopyable() {}

private:
	NonCopyable(NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
};