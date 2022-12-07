#include "Material.h"
#include "TextureManager.h"

std::unique_ptr<Material> Material::Create()
{
	//struct impl : public Material
	//{
	//	impl() {}
	//};
	//
	//return std::make_unique<impl>();

	return std::unique_ptr<Material>(new Material);
}

void Material::CreateBuffer(ID3D12Device* dev)
{
	m_constBuff.Create(dev);
	m_constBuff.Map();
	m_constBuff.MapPtr()->ambient = m_ambient;
	m_constBuff.MapPtr()->deffuse = m_diffuse;
	m_constBuff.MapPtr()->shininess = m_shininess;
	m_constBuff.MapPtr()->specular = m_specular;
	m_constBuff.MapPtr()->alpha = m_alpha;
}

void Material::SetGraphicsCommand(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndexMaterial, UINT rootParameterIndexTexture)
{
	m_constBuff.SetGraphicsRootConstantBufferView(cmdList, rootParameterIndexMaterial);
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(cmdList, rootParameterIndexTexture, m_textureHandle);

}
