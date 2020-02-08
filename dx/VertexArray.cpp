#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#include "VertexArray.h"
#include "Logger.h"
#include "std/map.h"

inline static Map<uint, ID3D11InputLayout*> layout;
struct vao_data
{
	Array<ID3D11Buffer*>			buffers;
	Array<uint>						strides;
	Array<uint>						offsets;
};
inline static Array<vao_data*> vaos;

VertexArray::VertexArray() :
	id(-1),
	size(0)
{
}

void VertexArray::Generate()
{
	vaos.push(new vao_data);
	id = global_id++;
}

void VertexArray::Bind()
{
	Graphics::context->IASetVertexBuffers(0,
		vaos[id]->buffers.size(),
		vaos[id]->buffers.data(),
		vaos[id]->strides.data(),
		vaos[id]->offsets.data());
}

void VertexArray::Draw()
{
	Bind();
	Graphics::context->Draw(size, 0);
}


void VertexArray::CreateVertexBuffer(uint stride, uint byteWidth, void* memptr)
{
	static D3D11_BUFFER_DESC	DESC = {
	0, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	static D3D11_SUBRESOURCE_DATA SUBDATA = { 0, 0, 0 };
	DESC.ByteWidth = byteWidth;
	SUBDATA.pSysMem = memptr;
	ID3D11Buffer* buffer;
	log(Graphics::device->CreateBuffer(&DESC, &SUBDATA, &buffer),
		"Failed to create vertex buffer.");
	vaos[id]->buffers.push(buffer);
	vaos[id]->strides.push(stride);
	vaos[id]->offsets.push(0);
}

void VertexArray::UpdateVertexBuffer(uint stride, uint byteWidth, void* memptr, uint buffer_index)
{
	D3D11_MAPPED_SUBRESOURCE subdata{ memptr };
	subdata.pData = memptr;
	subdata.RowPitch = byteWidth;
	subdata.DepthPitch = 1;
	vaos[id]->strides[id] = stride;
	Graphics::context->Map(vaos[id]->buffers[buffer_index], 0, D3D11_MAP_WRITE_DISCARD, 0, &subdata);
}




