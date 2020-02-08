#pragma once
#include "array.h"
template<class T>
class List : public Array<T>
{
protected:
	using arr = Array<T>;
	using arr::arr;
	using arr::dat, arr::siz, arr::cap;
	T* copy() const;
public:
	~List();
	List(const List& v);
	List(List&& v);
	List& operator =(const List& v);
	List& operator =(List&& v);
	explicit List(uint n);
	void clear();
	T pop();
	List& resize(unsigned n);
};

template<class T>
class Stack : public List<T>
{
	using lst = List<T>;
	using lst::lst;
	using lst::dat, lst::siz, lst::cap;
	using lst::arr::alloc;
	T*& release();
	T* mov(unsigned n);
	T* realloc();
	void _umove(T* dst, T* src, unsigned len);
	void shiftright1(T* block, unsigned len);
	void shiftright(T* block, unsigned len, unsigned shift);
	T* split(unsigned p);
public:
	void shrink();
	void reserve(unsigned n);
	void resize(unsigned n);
	void push(const T& item);
	void push(T&& item);
	template<class... Ts>
	void emplace(const Ts& ...ts);
	T* insert(T* where, const T& item);
	T* insert(T* where, T&& item);
};

template<class T>
inline T* List<T>::copy() const
{
	T* re = arr::alloc(siz);
	for (unsigned i = 0; i < siz; ++i)
		new(re + i) T(dat[i]);
	return re;
}

template<class T>
inline List<T>::~List()
{
	clear();
}

template<class T>
inline List<T>::List(const List& v) :
	Array<T>(v.copy(), v.siz, v.siz)
{
}

template<class T>
inline List<T>::List(List&& v) :
	Array<T>(v.dat, v.siz, v.cap)
{
	v.dat = nullptr;
	v.siz = v.cap = 0;
}

template<class T>
inline List<T>& List<T>::operator=(const List& v)
{
	if (this == &v) return *this;
	clear();
	free(dat);
	dat = v.copy();
	siz = v.siz;
	cap = v.siz;
	return *this;
}

template<class T>
inline List<T>& List<T>::operator=(List&& v)
{
	dat = v.dat;
	siz = v.siz;
	cap = v.cap;
	v.dat = nullptr;
	v.siz = v.cap = 0;
	return *this;
}

template<class T>
inline List<T>::List(uint n)
{
	resize(n);
}

template<class T>
inline void List<T>::clear()
{
	for (uint i = 0; i < siz; ++i)
		dat[i].~T();
	siz = 0;
}

template<class T>
inline T List<T>::pop()
{
	if (arr::empty()) throw;
	return (T&&)dat[--siz];
}

template<class T>
inline List<T>& List<T>::resize(unsigned n)
{
	if (n <= siz) return *this;
	arr::reserve(n);
	while (siz < n) new(dat + siz++) T();
	return *this;
}



template<class T>
inline T*& Stack<T>::release()
{
	for (int i = 0; i < siz; ++i)
		dat[i].~T();
	free(dat);
	return dat;
}

template<class T>
inline T* Stack<T>::mov(unsigned n)
{
	T* re = alloc(n);
	for (unsigned i = 0; i < siz; ++i)
		new(re + i) T((T&&)dat[i]);
	return re;
}

template<class T>
inline T* Stack<T>::realloc()
{
	if (siz == cap)
		release() = mov(++cap *= 2);
	return dat;
}

template<class T>
inline void Stack<T>::_umove(T* dst, T* src, unsigned len)
{
	while (len--)
	{
		new(dst++) T((T&&)*src);
		src++->~T();
	}
}

template<class T>
inline void Stack<T>::shiftright1(T* block, unsigned len)
{
	new(block + len) T((T&&)block[--len]);
	for (int i = len; i > 0; --i)
		block[i] = (T&&)block[i - 1];
	block->~T();
}

template<class T>
inline void Stack<T>::shiftright(T* block, unsigned len, unsigned shift)
{
	int delta = shift;
	while (shift--) new(block + len) T((T&&)block[len-- - delta]);
	while (len)		block[len] = (T&&)block[len - delta], --len;
	while (delta)	block[--delta].~T();
}

template<class T>
inline T* Stack<T>::split(unsigned p)
{
	if (siz == cap)
	{
		T* temp = alloc(++cap *= 2);
		_umove(temp + p + 1, dat + p, siz - p);
		_umove(temp, dat, p);
		free(dat);
		dat = temp;
	}
	else shiftright(dat + p, siz - p, 1);
	++siz;
	return dat + p;
}

template<class T>
inline void Stack<T>::shrink()
{
	if (siz == cap) return;
	release() = mov(cap = siz);
}

template<class T>
inline void Stack<T>::reserve(unsigned n)
{
	if (n <= cap) return;
	release() = mov(cap = n);
}

template<class T>
inline void Stack<T>::resize(unsigned n)
{
	if (n <= siz) return;
	reserve(n);
	while (siz <= n) new(dat + siz++) T;
}

template<class T>
inline void Stack<T>::push(const T& item)
{
	new(realloc() + siz++) T(item);
}

template<class T>
inline void Stack<T>::push(T&& item)
{
	new(realloc() + siz++) T((T&&)item);
}

template<class T>
template<class...Ts>
inline void Stack<T>::emplace(const Ts&...ts)
{
	new(realloc() + siz++) T(ts...);
}

template<class T>
inline T* Stack<T>::insert(T* where, const T& item)
{
	if (where < dat || where > lst::arr::end()) throw;
	return new(split(where - dat)) T(item);
}

template<class T>
inline T* Stack<T>::insert(T* where, T&& item)
{
	if (where < dat || where > lst::arr::end()) throw;
	new(split(where - dat)) T((T&&)item);
}
