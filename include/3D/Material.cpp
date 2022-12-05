#include "Material.h"
#include "TextureManager.h"

std::unique_ptr<Material> Material::Create(std::string name)
{
	return std::make_unique<Material>(name);
}

void Material::CreateBuffer(ID3D12Device* dev)
{
	m_constBuff.Create(dev);
	m_constBuff.Map();
	m_constBuff.MapPtr()->ambient = m_ambient;
	m_constBuff.MapPtr()->deffuse = m_diffuse;
	m_constBuff.MapPtr()->specular = m_specular;
	m_constBuff.MapPtr()->alpha = m_alpha;
}

void Material::SetGraphicsCommand(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndexMaterial, UINT rootParameterIndexTexture)
{
	m_constBuff.SetGraphicsRootConstantBufferView(cmdList, rootParameterIndexMaterial);
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(cmdList, rootParameterIndexTexture, m_textureHandle);

}
