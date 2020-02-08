#pragma once
#include <fstream>
#include "vec.h"

class String
{
	using uint = unsigned;
	char* dat;
	uint siz, cap;
	char* alloc(uint n) const;
	char* copy() const;
	char* copy(uint n) const;
	char*& free();
	void realloc();
	void realloc(uint n);
public:
	~String();
	String();
	explicit String(uint n);
	String(const char* str);
	String(const String& str);
	String(String&& str) noexcept;
	String& operator=(const String& str);
	String& operator=(String&& str) noexcept;
	String& operator+=(char c);
	String& operator+=(const char* str);
	String& operator+=(const wchar_t* str);
	String& operator+=(const String& str);
	String& Trim();
	String Substring(uint pos, uint len) const;
	String Substring(uint pos) const;
	bool Contains(char c) const;
	bool Contains(const String& str);
	uint IndexOf(char c, uint start = 0) const;
	uint IndexOf(const String& str, uint start = 0) const;
	String& Remove(uint pos);
	String& Remove(uint pos, uint len);
	bool operator==(const String& str) const;
	bool operator<=(const String& str) const;
	String& ReadAllText(const char* path);
	std::istream& getline(std::istream& s, char delim = '\n');
	String operator+(const char* str) const;
	String& operator>>(long& n);
	int		toint() const;
	uint	touint() const;
	float	tofloat() const;
	double	todouble() const;
	String& operator>>(int& n);
	String& operator>>(uint& n);
	String& operator>>(float& n);
	String& operator>>(double& n);
	uint size() const;
	const char* c_str() const;
	explicit operator const char*() const;
	char* data();
	const char* begin() const;
	const char* end() const;
	char& operator[](uint n);
	const char& operator[](uint n) const;
	friend std::ostream& operator << (std::ostream& stream, const String& str);
	friend String tostr(float);
	friend String tostr(double);
	friend String tostr(int);
	friend String tostr(unsigned);
	friend String tostr(unsigned long);
};



inline char* String::alloc(uint n) const
{
	return ::new char[n];
}

inline char* String::copy() const
{
	return (char*)memcpy(alloc(siz + 1), dat, siz + 1);
}

inline char* String::copy(uint n) const
{
	return (char*)memcpy(alloc(n), dat, siz);
}

inline char*& String::free()
{
	delete[] dat;
	return dat;
}

inline void String::realloc()
{
	if (siz + 1 != cap) return;
	dat = (char*)::realloc(dat, ++cap *= 2);
}

inline void String::realloc(uint n)
{
	if (n + 1 <= cap) return;
	dat = (char*)::realloc(dat, cap = n + 1);
}

inline String::~String()
{
	delete[] dat;
}

inline String::String() :
	dat(alloc(1)),
	siz(0),
	cap(1)
{
	*dat = 0;
}

inline String::String(uint n) :
	dat(alloc(n + 1)),
	siz(n),
	cap(n + 1)
{
	dat[siz] = 0;
}

inline String::String(const char* str)
{
	siz = strlen(str);
	cap = siz + 1;
	dat = (char*)memcpy(alloc(cap), str, cap);
}

inline String::String(const String& str) :
	dat(str.copy()),
	siz(str.siz),
	cap(str.siz + 1)
{
}

inline String::String(String&& str) noexcept :
	dat(str.dat),
	siz(str.siz),
	cap(str.cap)
{
	str.dat = nullptr;
	str.siz = str.cap = 0;
}

inline String& String::operator = (const String& str)
{
	if (this == &str) return *this;
	free() = str.copy();
	siz = str.siz;
	cap = siz + 1;
	return *this;
}

inline String& String::operator = (String&& str) noexcept
{
	free() = str.dat;
	siz = str.siz;
	cap = str.siz;
	str.dat = nullptr;
	str.siz = str.cap = 0;
	return *this;
}

inline String& String::operator +=(char c)
{
	realloc();
	dat[siz] = c;
	dat[++siz] = 0;
	return *this;
}

inline String& String::operator +=(const char* str)
{
	uint len = strlen(str);
	realloc(siz + len);
	memcpy(dat + siz, str, len);
	siz += len;
	dat[siz] = 0;
	return *this;
}

inline String& String::operator +=(const wchar_t* str)
{
	uint i = 0;
	while (str[i] != 0) operator+=(char(str[i++]));
	return *this;
}

inline String& String::operator += (const String& str)
{
	realloc(siz + str.siz);
	memcpy(dat + siz, str.dat, str.siz);
	siz += str.siz;
	dat[siz] = 0;
	return *this;
}

inline String& String::Trim()
{
	if (!siz) return *this;
	uint i = 0, j = siz - 1;
	while (dat[i] == ' ' ||
		dat[i] == '\f' ||
		dat[i] == '\n' ||
		dat[i] == '\r' ||
		dat[i] == '\t' ||
		dat[i] == '\v') ++i;
	while (dat[j] == ' ' ||
		dat[j] == '\f' ||
		dat[j] == '\n' ||
		dat[j] == '\r' ||
		dat[j] == '\t' ||
		dat[j] == '\v') --j;
	j = siz - j - 1;
	memcpy(dat, dat + i, siz - i - j);
	siz -= (i + j);
	dat[siz] = 0;
	return *this;
}

inline String String::Substring(uint pos, uint len) const
{
	String re(len);
	memcpy(re.dat, dat + pos, len);
	return re;
}

inline String String::Substring(uint pos) const
{
	return Substring(pos, siz - pos);
}

inline bool String::Contains(char c) const
{
	uint i = 0;
	while (i < siz)
		if (dat[i++] == c) return true;
	return false;
}

inline bool String::Contains(const String& str)
{
	if (str.siz > siz) return false;
	for (uint i = 0; i < siz; ++i)
	{
		if (dat[i] == str.dat[0])
		{
			bool flag = 1;
			for (uint j = 1; j < str.siz; ++j)
			{
				if (dat[i + j] != str.dat[j])
				{
					flag = 0;
					break;
				}
			}
			if (flag) return true;
		}
	}
	return false;
}

inline uint String::IndexOf(char c, uint start) const
{
	for (uint i = start; i < siz; ++i)
		if (dat[i] == c) return i;
	return -1;
}

inline uint String::IndexOf(const String& str, uint start) const
{
	if (str.siz > siz) return -1;
	for (uint i = 0; i < siz; ++i)
	{
		if (dat[i] == str.dat[0])
		{
			bool flag = 1;
			for (uint j = 1; j < str.siz; ++j)
			{
				if (dat[i + j] != str.dat[j])
				{
					flag = 0;
					break;
				}
			}
			if (flag) return i;
		}
	}
	return -1;
}

inline String& String::Remove(uint pos)
{
	siz -= siz - pos;
	dat[siz] = 0;
	return *this;
}

inline String& String::Remove(uint pos, uint len)
{
	memcpy(dat + pos, dat + pos + len, siz - len - pos);
	siz -= len;
	dat[siz] = 0;
	return *this;
}

inline bool String::operator == (const String& str) const
{
	return !strcmp(dat, str.dat);
}

inline bool String::operator <= (const String& str) const
{
	return strcmp(dat, str.dat) <= 0;
}

inline String& String::ReadAllText(const char* path)
{
	std::ifstream f(path);
	char c;
	while (f.get(c)) operator+=(c);
	f.close();
	return *this;
}

inline std::istream& String::getline(std::istream& s, char delim)
{
	siz = 0;
	static char c;
	while (s.get(c) && c != delim) operator+=(c);
	return s;
}

inline String String::operator + (const char* str) const
{
	return String(*this) += str;
}

inline String& String::operator>>(long& n)
{
	static char* end;
	n = strtol(dat, &end, 10);
	Remove(0, end - dat);
	return *this;
}

inline int String::toint() const
{
	return strtol(dat, 0, 10);
}

inline uint String::touint() const
{
	return strtoul(dat, 0, 10);
}

inline float String::tofloat() const
{
	return strtof(dat, 0);
}

inline double String::todouble() const
{
	return strtod(dat, 0);
}

inline String& String::operator>>(int& n)
{
	static char* end;
	n = strtoul(dat, &end, 10);
	Remove(0, end - dat);
	return *this;
}

inline String& String::operator>>(uint& n)
{
	static char* end;
	n = strtoul(dat, &end, 10);
	Remove(0, end - dat);
	return *this;
}

inline String& String::operator>>(float& n)
{
	static char* end;
	n = strtof(dat, &end);
	Remove(0, end - dat);
	return *this;
}

inline String& String::operator>>(double& n)
{
	static char* end;
	n = strtod(dat, &end);
	Remove(0, end - dat);
	return *this;
}

inline uint String::size() const
{
	return siz;
}

inline std::ostream& operator << (std::ostream& stream, const String& str)
{
	return stream << str.dat;
}

inline const char* String::c_str() const
{
	return dat;
}

inline String::operator const char* () const
{
	return dat;
}

inline char* String::data()
{
	return dat;
}

inline const char* String::begin() const
{
	return dat;
}

inline const char* String::end() const
{
	return dat + siz;
}

inline char& String::operator[](uint n)
{
	return dat[n];
}

inline const char& String::operator[](uint n) const
{
	return dat[n];
}


inline String tostr(float x) {
	String re(16);
	re.siz = snprintf(re.dat, 16, "%f", x);
	return re;
}

inline String tostr(double x) {
	String re(32);
	re.siz = snprintf(re.dat, 32, "%f", x);
	return re;
}

inline String tostr(int x) {
	String re(16);
	re.siz = snprintf(re.dat, 16, "%d", x);
	return re;
}

inline String tostr(unsigned x) {
	String re(16);
	re.siz = snprintf(re.dat, 16, "%u", x);
	return re;
}

inline String tostr(unsigned long x) {
	String re(32);
	re.siz = snprintf(re.dat, 32, "%ul", x);
	return re;
}

template<class T, uint...x>
inline String tostr(const base<T, x...>& v)
{
	return ((tostr(*(T*)(&v + x)) += ' ') += ...);
}