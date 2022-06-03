#include "Terrapch.h"
#include "Plane.h"

Terra::Plane Terra::Plane::Create()
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
		DirectX::XMFLOAT2 tex;
	};

	std::vector<Vertex> vertices(4);
	vertices[0].pos =	{ -0.75f, -0.75f, 0.0f };
	vertices[0].n =		{ 0.0f, 0.0f, -1.0f };
	vertices[0].tex =	{ 0.0f, 0.0f };
	
	vertices[1].pos =	{ 0.75f, -0.75f, 0.0f };
	vertices[1].n =		{ 0.0f, 0.0f, -1.0f };
	vertices[1].tex =	{ 1.0f, 0.0f };
	
	vertices[2].pos =	{ 0.75f, 0.75f, 0.0f };
	vertices[2].n =		{ 0.0f, 0.0f, -1.0f };
	vertices[2].tex =	{ 0.0f, 1.0f };
	
	vertices[3].pos =	{ -0.75f, 0.75f, 0.0f };
	vertices[3].n =		{ 0.0f, 0.0f, -1.0f };
	vertices[3].tex =	{ 1.0f, 1.0f };
	
	
	constexpr size_t elementCount = sizeof(Vertex) / sizeof(float);
	std::vector<float> v_out(elementCount * vertices.size());

	memcpy(v_out.data(), vertices.data(), sizeof(Vertex) * vertices.size());
	std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
	return { v_out,  indices};
}
