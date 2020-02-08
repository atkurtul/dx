#include "Logger.h"
#include <comdef.h>
#include "std/string.h"
#include <xmemory>

void log(long hr, const char* error)
{
	if (hr < 0)
		MessageBoxA(NULL, (String(error) += _com_error(hr).ErrorMessage()).c_str(), "Error", MB_ICONERROR);
}

wstr::~wstr()
{
	delete[] dat;
}

wstr::wstr(const char* str)
{
	int len = strlen(str) + 1;
	dat = new wchar_t[len];
	while (len--) dat[len] = str[len];
}

wstr::operator const wchar_t* () const
{
	return dat;
}