#include "Meth.h"
#include "Material.h"

std::unique_ptr<Meth> Meth::Create()
{
	//struct impl : public Meth
	//{
	//	impl() {}
	//};
	//
	//return std::make_unique<impl>();

	return std::unique_ptr<Meth>(new Meth);
}

void Meth::CreateBuffer(ID3D12Device* dev)
{

	// 頂点バッファを生成
	m_vertBuff.Create(dev, (UINT)m_vertcies.size());
	m_vertBuff.Map();
	std::copy(m_vertcies.begin(), m_vertcies.end(), m_vertBuff.GetMapPtr());
	m_vertBuff.Unmap();
	// インデックスバッファを生成
	m_indexBuff.Create(dev, (UINT)m_indcies.size());
	m_indexBuff.Map();
	std::copy(m_indcies.begin(), m_indcies.end(), m_indexBuff.GetMapPtr());
	m_indexBuff.Unmap();
}

void Meth::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndexMaterial, UINT rootParameterIndexTexture)
{
	m_vertBuff.IASet(cmdList);
	m_indexBuff.IASet(cmdList);
	m_material->SetGraphicsCommand(cmdList, rootParameterIndexMaterial, rootParameterIndexTexture);
	cmdList->DrawIndexedInstanced(m_indexBuff.GetIndexCount(), 1, 0, 0, 0);
}
