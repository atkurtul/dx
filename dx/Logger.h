#pragma once

void log(long hr, const char* error);

class wstr
{
	wchar_t* dat;
public:
	~wstr();
	wstr(const char* str);
	operator const wchar_t* () const;
};