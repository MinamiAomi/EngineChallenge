#include "Meth.h"

std::unique_ptr<Meth> Meth::Create(Material* material)
{
	return std::make_unique<Meth>(material);
}

void Meth::CreateBuffer(ID3D12Device* dev)
{
	// 頂点バッファを生成
	m_vertBuff.Create(dev, m_vertcies.size());
	m_vertBuff.Map();
	std::copy(m_vertcies.begin(), m_vertcies.end(), m_vertBuff.GetMapPtr());
	m_vertBuff.Unmap();
	// インデックスバッファを生成
	m_indexBuff.Create(dev, m_indcies.size());
	m_indexBuff.Map();
	std::copy(m_indcies.begin(), m_indcies.end(), m_indexBuff.GetMapPtr());
	m_indexBuff.Unmap();
}

void Meth::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndexMaterial, UINT rootParameterIndexTexture)
{

	m_vertBuff.IASet(cmdList);
	m_indexBuff.IASet(cmdList);
}
