#pragma once
#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MathUtility.h"
#include <memory>

class Meth
{
	
public:
public:
	struct Vertex
	{
		Vector3 position;	// 座標
		Vector3 normal;		// 法線
		Vector2 uv;			// uv座標
	};

public:
	static std::unique_ptr<Meth> Create(class Material* material);

private:
	VertexBuffer<Vertex> m_vertBuff;
	IndexBuffer m_indexBuff;
	std::vector<Vertex> m_vertcies;
	std::vector<uint16_t> m_indcies;
	class Material* m_material = nullptr;

public:
	
	size_t GetVertexCount() const { return m_vertcies.size(); }
	size_t GetIndexCount() const { return m_indcies.size(); }

	void AddVertex(const Vertex& vertex) { m_vertcies.emplace_back(vertex); }
	void AddIndex(uint16_t index) { m_indcies.emplace_back(index); }

	void SetMaterial(Material* mate) { m_material = mate; }

	void CreateBuffer(ID3D12Device* dev);

	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndexMaterial, UINT rootParameterIndexTexture);

private:
	Meth(Material* material) : m_material(material) {}
};

