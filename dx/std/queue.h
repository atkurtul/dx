#pragma once

template<class T>
class Queue
{
protected:
	T* dat;
	unsigned siz, cap, head, rear;

	T* alloc(unsigned n) const;
	T*& release();
	void destruct();
	T* copy() const;
	T* fix_rear(T* buffer) const;
	T* shift_head(T* buffer) const;
	T* realloc();
	explicit Queue(T* dat, unsigned siz);
public:
	~Queue();
	Queue();
	Queue(const Queue& q);
	void que(const T& item);
	void push(const T& item);
	T pop();
	T* begin();
	T* end();
};

template<class T>
inline T* Queue<T>::alloc(unsigned n) const
{
	return (T*)malloc(n * sizeof(T));
}

template<class T>
inline T*& Queue<T>::release()
{
	free(dat);
	return dat;
}

template<class T>
inline void Queue<T>::destruct()
{
	if (siz)
	{
		if (rear > head)
			for (; head != rear; ++head)
				dat[head].~T();
		else
		{
			for (unsigned i = 0; i < rear; ++i)
				dat[i].~T();
			for (unsigned i = head; i < cap; ++i)
				dat[i].~T();
		}
	}
}

template<class T>
inline T* Queue<T>::copy() const
{
	if (rear > cap)
		return shift_head(alloc(siz));
	else
		return fix_rear(alloc(siz));
}

template<class T>
inline T* Queue<T>::fix_rear(T* buffer) const
{
	unsigned dd = cap - head;
	memcpy(buffer, dat + head, dd * sizeof(T));
	memcpy(buffer + dd, dat, rear * sizeof(T));
	return buffer;
}

template<class T>
inline T* Queue<T>::shift_head(T* buffer) const
{
	return (T*)memcpy(buffer, dat + head, siz * sizeof(T));
}

template<class T>
inline T* Queue<T>::realloc()
{
	if (siz < cap) return dat + rear;
	if (rear > head)
		release() = shift_head(alloc(++cap *= 2)),
		rear -= head;
	else
		release() = fix_rear(alloc(cap * 2 + 2)),
		++cap *= 2;
	rear = siz;
	head = 0;
	return dat + rear;
}

template<class T>
inline Queue<T>::Queue(T* dat, unsigned siz) :
	dat(dat),
	siz(siz),
	cap(siz),
	head(0),
	rear(siz)
{
}

template<class T>
inline Queue<T>::~Queue()
{
	destruct();
	release();
}

template<class T>
inline Queue<T>::Queue() :
	dat(0),
	siz(0),
	cap(0),
	head(0),
	rear(0)
{
}

template<class T>
inline Queue<T>::Queue(const Queue& q) :
	Queue(q.alloc(q.siz), q.siz)
{
}


template<class T>
inline void Queue<T>::que(const T& item)
{
	if (!siz) head = rear = 0;
	new(realloc())T(item);
	++siz;
	++rear %= cap + 1;
}

template<class T>
inline void Queue<T>::push(const T& item)
{
	if (!siz) head = rear = 0;
	new(realloc())T(item);
	++siz;
	++rear %= cap + 1;
}

template<class T>
inline T Queue<T>::pop()
{
	--siz;
	return (T&&)dat[head++];
}

template<class T>
inline T* Queue<T>::begin()
{
	return dat + head;
}

template<class T>
inline T* Queue<T>::end()
{
	return dat + rear;
}
