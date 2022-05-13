#include "Terrapch.h"
#include "Sphere.h"

Terra::Sphere::~Sphere()
{
}

Terra::Sphere Terra::Sphere::Create()
{
	return CreateTesselated(12, 24);
}

Terra::Sphere Terra::Sphere::CreateNormalized()
{
	return CreateTesselatedNormalized(12, 24);
}

Terra::Sphere Terra::Sphere::CreateTesselated(uint32_t latDiv, uint32_t longDiv)
{
	TERRA_ASSERT(latDiv >= 3u && longDiv >= 3u);
	constexpr float radius = 1.0f;
	constexpr float PI = 3.14159265f;
	const auto base = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
	const float lattitudeAngle = PI / latDiv;
	const float longitubeAngle = 2.0f * PI / longDiv;

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
	};

	std::vector<Vertex> vertices;
	size_t capacity = (size_t)((latDiv - 1u) * longDiv + 6);
	vertices.reserve(capacity);

	for (int iLat = 1; iLat < latDiv; iLat++)
	{
		const auto latBase = DirectX::XMVector3Transform(base,
			DirectX::XMMatrixRotationX(lattitudeAngle * iLat));
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = DirectX::XMVector3Transform(latBase,
				DirectX::XMMatrixRotationZ(longitubeAngle * iLong));
			DirectX::XMStoreFloat3(&vertices.back().pos, v);
		}
	}

	// Add cap vertices
	vertices.emplace_back();
	DirectX::XMStoreFloat3(&vertices.back().pos, base);
	vertices.emplace_back();
	DirectX::XMStoreFloat3(&vertices.back().pos, DirectX::XMVectorNegate(base));

	std::vector<uint32_t> indices;
	FillIndices(indices, latDiv, longDiv, vertices.size());

	constexpr size_t elementCount = sizeof(Vertex) / sizeof(float);
	std::vector<float> v_out(elementCount* vertices.size());

	memcpy(v_out.data(), vertices.data(), sizeof(Vertex)* vertices.size());

	return { v_out, indices };
}


Terra::Sphere Terra::Sphere::CreateTesselatedNormalized(uint32_t latDiv, uint32_t longDiv)
{
	SB_ASSERT(latDiv >= 3u && longDiv >= 3u);
	constexpr float radius = 1.0f;
	constexpr float PI = 3.14159265f;
	const auto base = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
	const float lattitudeAngle = PI / latDiv;
	const float longitubeAngle = 2.0f * PI / longDiv;

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
	};

	std::vector<Vertex> vertices;
	size_t capacity = (size_t)((latDiv - 1u) * longDiv + 6);
	vertices.reserve(capacity);

	for (int iLat = 1; iLat < latDiv; iLat++)
	{
		const auto latBase = DirectX::XMVector3Transform(base,
			DirectX::XMMatrixRotationX(lattitudeAngle * iLat));
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();		
			auto v = DirectX::XMVector3Transform(latBase,
				DirectX::XMMatrixRotationZ(longitubeAngle * iLong));
			DirectX::XMStoreFloat3(&vertices.back().pos, v);
			
			auto n = DirectX::XMVector3Normalize(v);
			DirectX::XMStoreFloat3(&vertices.back().n, n);
		}
	}

	// Add cap vertices
	vertices.emplace_back();
	DirectX::XMStoreFloat3(&vertices.back().pos, base);
	DirectX::XMStoreFloat3(&vertices.back().n, DirectX::XMVector3Normalize(base));
	
	vertices.emplace_back();
	DirectX::XMStoreFloat3(&vertices.back().pos, DirectX::XMVectorNegate(base));
	DirectX::XMStoreFloat3(&vertices.back().n, DirectX::XMVector3Normalize(DirectX::XMVectorNegate(base)));

	std::vector<uint32_t> indices;
	FillIndices(indices, latDiv, longDiv, vertices.size()); 
	
	constexpr size_t elementCount = sizeof(Vertex) / sizeof(float);
	std::vector<float> v_out(elementCount * vertices.size());

	memcpy(v_out.data(), vertices.data(), sizeof(Vertex) * vertices.size());

	return { v_out, indices };
}

void Terra::Sphere::FillIndices(std::vector<uint32_t>& indices_in, uint32_t latDiv, uint32_t longDiv, uint32_t vertexCount)
{
	const auto calcIndex = [latDiv, longDiv](uint32_t iLat, uint32_t iLong)
	{ return iLat * longDiv + iLong; };

	const auto capacity = (size_t)((latDiv - 2u) * (2u * (longDiv - 1u)));
	indices_in.reserve(capacity);

	for (uint32_t iLat = 0; iLat < latDiv - 2; iLat++)
	{
		for (uint32_t iLong = 0; iLong < longDiv - 1; iLong++)
		{
			indices_in.emplace_back(calcIndex(iLat, iLong));
			indices_in.emplace_back(calcIndex(iLat + 1, iLong));
			indices_in.emplace_back(calcIndex(iLat, iLong + 1));
			indices_in.emplace_back(calcIndex(iLat, iLong + 1));
			indices_in.emplace_back(calcIndex(iLat + 1, iLong));
			indices_in.emplace_back(calcIndex(iLat + 1, iLong + 1));
		}
		// wrap band
		indices_in.emplace_back(calcIndex(iLat, longDiv - 1));
		indices_in.emplace_back(calcIndex(iLat + 1, longDiv - 1));
		indices_in.emplace_back(calcIndex(iLat, 0));
		indices_in.emplace_back(calcIndex(iLat, 0));
		indices_in.emplace_back(calcIndex(iLat + 1, longDiv - 1));
		indices_in.emplace_back(calcIndex(iLat + 1, 0));
	}

	// cap fans
	const auto iNorthPole = (uint32_t)(vertexCount - 2u);
	const auto iSouthPole = (uint32_t)(vertexCount - 1u);
	for (uint32_t iLong = 0; iLong < longDiv - 1; iLong++)
	{
		// north
		indices_in.emplace_back(iNorthPole);
		indices_in.emplace_back(calcIndex(0, iLong));
		indices_in.emplace_back(calcIndex(0, iLong + 1));

		// south
		indices_in.emplace_back(calcIndex(latDiv - 2, iLong + 1));
		indices_in.emplace_back(calcIndex(latDiv - 2, iLong));
		indices_in.emplace_back(iSouthPole);
	}

	// wrap triangles
	// north
	indices_in.emplace_back(iNorthPole);
	indices_in.emplace_back(calcIndex(0, longDiv - 1));
	indices_in.emplace_back(calcIndex(0, 0));
	// south
	indices_in.emplace_back(calcIndex(latDiv - 2, 0));
	indices_in.emplace_back(calcIndex(latDiv - 2, longDiv - 1));
	indices_in.emplace_back(iSouthPole);
}

