#include "Terrapch.h"
#include "Cube.h"


Terra::Cube Terra::Cube::CreateSolid()
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
	};
	constexpr float side = 1.0 / 2.0f;
	std::vector<Vertex> vertices(24);
	vertices[0].pos = { -side,-side,-side };// 0 near side
	vertices[1].pos = { side,-side,-side };// 1
	vertices[2].pos = { -side,side,-side };// 2
	vertices[3].pos = { side,side,-side };// 3
	vertices[4].pos = { -side,-side,side };// 4 far side
	vertices[5].pos = { side,-side,side };// 5
	vertices[6].pos = { -side,side,side };// 6
	vertices[7].pos = { side,side,side };// 7
	vertices[8].pos = { -side,-side,-side };// 8 left side
	vertices[9].pos = { -side,side,-side };// 9
	vertices[10].pos = { -side,-side,side };// 10
	vertices[11].pos = { -side,side,side };// 11
	vertices[12].pos = { side,-side,-side };// 12 right side
	vertices[13].pos = { side,side,-side };// 13
	vertices[14].pos = { side,-side,side };// 14
	vertices[15].pos = { side,side,side };// 15
	vertices[16].pos = { -side,-side,-side };// 16 bottom side
	vertices[17].pos = { side,-side,-side };// 17
	vertices[18].pos = { -side,-side,side };// 18
	vertices[19].pos = { side,-side,side };// 19
	vertices[20].pos = { -side,side,-side };// 20 top side
	vertices[21].pos = { side,side,-side };// 21
	vertices[22].pos = { -side,side,side };// 22
	vertices[23].pos = { side,side,side };// 23

	std::vector<uint32_t> indices =
	{
		0,2, 1,    2,3,1,
		4,5, 7,    4,7,6,
		8,10, 9,  10,11,9,
		12,13,15, 12,15,14,
		16,17,18, 18,17,19,
		20,23,21, 20,22,23
	};

	SetNormals(vertices, indices);

	constexpr size_t elementCount = sizeof(Vertex) / sizeof(float);
	std::vector<float> v_out(vertices.size() * elementCount);
	size_t size = vertices.size() * sizeof(Vertex);
	memcpy(v_out.data(), vertices.data(), size);

	return { v_out, indices };
}

Terra::Cube Terra::Cube::Create()
{
	SB_PROFILE_FUNCTION();
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
		DirectX::XMFLOAT2 tex;
	};
	
	constexpr float side = 1.0 / 2.0f;
	std::vector<Vertex> vertices(24);
	vertices[0].pos =	{ -side,-side,-side };// 0 near side
	vertices[1].pos =	{ side,-side,-side };// 1
	vertices[2].pos =	{ -side,side,-side };// 2
	vertices[3].pos =	{ side,side,-side };// 3
	vertices[4].pos =	{ -side,-side,side };// 4 far side
	vertices[5].pos =	{ side,-side,side };// 5
	vertices[6].pos =	{ -side,side,side };// 6
	vertices[7].pos =	{ side,side,side };// 7
	vertices[8].pos =	{ -side,-side,-side };// 8 left side
	vertices[9].pos =	{ -side,side,-side };// 9
	vertices[10].pos =	{ -side,-side,side };// 10
	vertices[11].pos =	{ -side,side,side };// 11
	vertices[12].pos =	{ side,-side,-side };// 12 right side
	vertices[13].pos =	{ side,side,-side };// 13
	vertices[14].pos =	{ side,-side,side };// 14
	vertices[15].pos =	{ side,side,side };// 15
	vertices[16].pos =	{ -side,-side,-side };// 16 bottom side
	vertices[17].pos =	{ side,-side,-side };// 17
	vertices[18].pos =	{ -side,-side,side };// 18
	vertices[19].pos =	{ side,-side,side };// 19
	vertices[20].pos =	{ -side,side,-side };// 20 top side
	vertices[21].pos =	{ side,side,-side };// 21
	vertices[22].pos =	{ -side,side,side };// 22
	vertices[23].pos =	{ side,side,side };// 23

	std::vector<uint32_t> indices =
	{
		0,2, 1,    2,3,1,
		4,5, 7,    4,7,6,
		8,10, 9,  10,11,9,
		12,13,15, 12,15,14,
		16,17,18, 18,17,19,
		20,23,21, 20,22,23
	};

	SetNormals(vertices, indices);
	SetTextureCoordinates(vertices);
	auto a = std::size(vertices);
	constexpr size_t elementCount = sizeof(Vertex) / sizeof(float);
	std::vector<float> v_out(vertices.size() * elementCount);
	size_t size = vertices.size() * sizeof(Vertex);
	memcpy(v_out.data(), vertices.data(), size);

	return { v_out, indices };
}