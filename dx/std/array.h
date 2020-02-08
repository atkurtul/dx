#pragma once
#include <xmemory>
#define XMOVE(x) (decltype(x)&&)x
using uint = unsigned;
template<class T>
class Array
{
protected:
	T* dat;
	uint siz, cap;
	T* alloc(uint n) const;
	T*& release();
	T* copy(uint n) const;
	T* realloc();
	T* split(uint p);
	explicit Array(T* cpy, uint siz, uint cap);
public:
	~Array();
	Array();
	explicit Array(uint n);
	Array(std::initializer_list<T>);
	Array(const Array& a);
	Array(Array&& a);
	Array<T>& operator=(const Array& a);
	Array<T>& operator=(Array&& a);
	T& operator[](unsigned n);
	T& at(unsigned n);
	T& front();
	T& back();
	const T& operator[](unsigned n) const;
	const T& at(unsigned n) const;
	const T& front() const;
	const T& back() const;
	void clear();
	void shrink();
	void reserve(unsigned n);
	Array& resize(unsigned n);
	bool empty() const;
	uint size() const;
	uint capacity() const;
	void push(const T& item);
	void push(T&& item);
	template<class... Ts>
	void emplace(const Ts& ...ts);
	T* insert(T* where, const T& item);
	T* insert(T* where, T&& item);
	T* begin();
	T* end();
	T* rbegin();
	T* rend();
	T* data();
	const T* begin() const;
	const T* end() const;
	const T* rbegin() const;
	const T* rend() const;
	const T* data() const;
	uint find(const T&) const;
	uint findif(bool(*f)(const T&)) const;
	Array& Remove(uint pos, uint len = 1);
};

template<class T>
inline T* Array<T>::alloc(uint n) const
{
	return (T*)malloc(n * sizeof(T));
}

template<class T>
inline T*& Array<T>::release()
{
	free(dat);
	return dat;
}

template<class T>
inline T* Array<T>::copy(uint n) const
{
	return (T*)memcpy(alloc(n), dat, siz * sizeof(T));
}

template<class T>
inline T* Array<T>::realloc()
{
	if (siz == cap)
		dat = (T*)::realloc(dat, (++cap *= 2) * sizeof(T));
	return dat;
}

template<class T>
inline T* Array<T>::split(uint p)
{
	if (siz == cap)
	{
		T* temp = alloc(++cap *= 2);
		memcpy(temp + p + 1, dat + p, (siz++ - p) * sizeof(T));
		memcpy(temp, dat, p * sizeof(T));
		return (release() = temp) + p;
	}
	else
		return (T*)memmove(dat + p + 1, dat + p, (siz++ - p) * sizeof(T)) - 1;
}

template<class T>
inline Array<T>::~Array()
{
	release();
}

template<class T>
inline Array<T>::Array(T* cpy, uint siz, uint cap) :
	dat(cpy),
	siz(siz),
	cap(cap)
{
}

template<class T>
inline Array<T>::Array() :
	dat(0),
	siz(0),
	cap(0)
{
}

template<class T>
inline Array<T>::Array(uint n) :
	dat(alloc(n)),
	siz(n),
	cap(n)
{
}

template<class T>
inline Array<T>::Array(std::initializer_list<T> list) :
	dat(alloc(list.size())),
	siz(list.size()),
	cap(siz)
{
	memcpy(dat, list.begin(), cap * sizeof(T));
}

template<class T>
inline Array<T>::Array(const Array& a) :
	dat(a.copy(a.siz)),
	siz(a.siz),
	cap(a.siz)
{
}

template<class T>
inline Array<T>::Array(Array&& a) :
	dat(a.dat),
	siz(a.siz),
	cap(a.cap)
{
	a.dat = nullptr;
	a.siz = a.cap = 0;
}

template<class T>
inline Array<T>& Array<T>::operator = (const Array& a)
{
	release() = a.copy(a.siz);
	cap = siz = a.siz;
	return *this;
}

template<class T>
inline Array<T>& Array<T>::operator = (Array&& a)
{
	release() = a.dat;
	siz = a.siz;
	cap = a.cap;
	a.dat = nullptr;
	a.siz = a.cap = 0;
	return *this;
}


template<class T>
inline T& Array<T>::operator[](unsigned n)
{
	//if (n >= siz) throw;
	return dat[n];
}

template<class T>
inline T& Array<T>::at(unsigned n)
{
	if (n >= siz) throw;
	return dat[n];
}

template<class T>
inline T& Array<T>::front()
{
	if (empty()) throw;
	return dat[0];
}

template<class T>
inline T& Array<T>::back()
{
	if (empty()) throw;
	return *rbegin();
}


template<class T>
inline const T& Array<T>::operator[](unsigned n) const
{
	//if (n >= siz) throw;
	return dat[n];
}

template<class T>
inline const T& Array<T>::at(unsigned n) const
{
	if (n >= siz) throw;
	return dat[n];
}

template<class T>
inline const T& Array<T>::front() const
{
	if (empty()) throw;
	return dat[0];
}

template<class T>
inline const T& Array<T>::back() const
{
	if (empty()) throw;
	return *rbegin();
}

template<class T>
inline void Array<T>::clear()
{
	siz = 0;
}

template<class T>
inline void Array<T>::shrink()
{
	if (siz == cap) return;
	release() = copy(cap = siz);
}

template<class T>
inline void Array<T>::reserve(unsigned n)
{
	if (n <= cap) return;
	dat = (T*)::realloc(dat, (cap = n) * sizeof(T));
}

template<class T>
inline Array<T>& Array<T>::resize(unsigned n)
{
	if (n <= siz) return *this;
	reserve(n);
	while (siz < n) new(dat + siz++) T();
	return *this;
}

template<class T>
inline bool Array<T>::empty() const
{
	return !siz;
}

template<class T>
inline uint Array<T>::size() const
{
	return siz;
}

template<class T>
inline uint Array<T>::capacity() const
{
	return cap;
}

template<class T>
inline void Array<T>::push(const T& item)
{
	new(realloc() + siz++) T(item);
}

template<class T>
inline void Array<T>::push(T&& item)
{
	new(realloc() + siz++) T((T&&)item);
}

template<class T>
template<class...Ts>
inline void Array<T>::emplace(const Ts&... ts)
{
	new(realloc() + siz++) T(ts...);
}

template<class T>
inline T* Array<T>::insert(T* where, const T& item)
{
	if (where < dat || where > end()) throw;
	return new(split(where - dat)) T(item);
}

template<class T>
inline T* Array<T>::insert(T* where, T&& item)
{
	if (where < dat || where > end()) throw;
	return new(split(where - dat)) T((T&&)item);
}

template<class T>
inline T* Array<T>::begin()
{
	return dat;
}

template<class T>
inline T* Array<T>::end()
{
	return dat + siz;
}

template<class T>
inline T* Array<T>::rbegin()
{
	return dat + siz - 1;
}

template<class T>
inline T* Array<T>::rend()
{
	return dat - 1;
}

template<class T>
inline T* Array<T>::data()
{
	return dat;
}

template<class T>
inline const T* Array<T>::begin() const
{
	return dat;
}

template<class T>
inline const T* Array<T>::end() const
{
	return dat + siz;
}

template<class T>
inline const T* Array<T>::rbegin() const
{
	return dat + siz - 1;
}

template<class T>
inline const T* Array<T>::rend() const
{
	return dat - 1;
}

template<class T>
inline const T* Array<T>::data() const
{
	return dat;
}

template<class T>
inline uint Array<T>::find(const T& item) const
{
	for (uint i = 0; i < siz; ++i)
		if (dat[i] == item) 
			return i;
	return -1;
}

template<class T>
inline uint Array<T>::findif(bool(*f)(const T&)) const
{
	for (uint i = 0; i < siz; ++i)
		if (f(dat[i]))
			return i;
	return -1;
}

template<class T>
inline Array<T>& Array<T>::Remove(uint pos, uint len)
{
	memcpy(dat + pos, dat + pos + len, (siz - len - pos) * sizeof(T));
	siz -= len;
	return *this;
}