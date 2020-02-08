#pragma once
#include "std/array.h"
#include "Graphics.h"

class VertexArray
{
	void CreateVertexBuffer(uint, uint, void*);
	void UpdateVertexBuffer(uint, uint, void*, uint);
	inline static uint global_id = 0;
public:
	uint id;
	uint size;
	VertexArray();
	void Generate();
	void Bind();
	void Draw();
	template<class T>
	void Buffer(const Array<T>&, uint = 1);
	template<class T, uint n>
	void Buffer(const T(&data)[n], uint stride);
	template<class T>
	void Update(const Array<T>&, uint = 1, uint = 0);
};

template<class T>
inline void VertexArray::Buffer(const Array<T>& data, uint stride)
{
	if (id == -1) Generate();
	if (size < data.size()) size = data.size();
	CreateVertexBuffer(stride * sizeof(T), data.size() * sizeof(T), (void*)data.data());
}

template<class T, uint n>
inline void VertexArray::Buffer(const T (&data)[n], uint stride)
{
	if (id == -1) Generate();
	if (size < n) size = n;
	CreateVertexBuffer(stride * sizeof(T), n * sizeof(T), (void*)data);
}

template<class T>
inline void VertexArray::Update(const Array<T>& data, uint stride, uint buffer_index)
{
	UpdateVertexBuffer(stride * sizeof(T), data.size() * sizeof(T), (void*)data.data(), buffer_index);
}

template<class T>
struct Uniform
{
	Uniform(const T& item, uint slot)
	{
		Graphics::CreateUniformBuffer(sizeof(T), (void*)&item, slot);
	}
	Uniform(const Array<T>& items, uint slot)
	{
		Graphics::CreateUniformBuffer(sizeof(T) * items.size(), (void*)items.data(), slot);
	}
};
