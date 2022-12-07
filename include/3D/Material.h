#pragma once
#include <string>
#include "MathUtility.h"
#include "ConstBuffer.h"
#include <memory>

class Material
{

	friend class Model;

private:
	struct ConstBufferDataMaterial 
	{
		Vector3 ambient;	// 環境光（アンビエント）
		float padding_1;	// パディング
		Vector3 deffuse;	// 拡散光（ディフューズ）
		float shininess;	// 光沢度
		Vector3 specular;	// 鏡面反射光（スペキュラー）
		float alpha;		// 透明度（アルファ）
	};

public:
	static std::unique_ptr<Material> Create();

private:
	std::string m_name;				// マテリアルの名前
	std::string m_texFileName;		// テクスチャの名前
	Vector3 m_ambient = { 0.3f,0.3f,0.3f };				// アンビエント
	Vector3 m_diffuse = { 0.0f,0.0f,0.0f };				// ディフューズ
	Vector3 m_specular = { 0.0f,0.0f,0.0f };			// スペキュラー
	float m_shininess = 5.0f;
	float m_alpha = 1.0f;			// 透明度

	ConstBuffer<ConstBufferDataMaterial> m_constBuff;
	UINT m_textureHandle = 0;

public:
	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }
	void SetTexFileName(const std::string& name) { m_texFileName = name; }
	const std::string& GetTexFileName() const { return m_texFileName; }
	void SetAmbient(const Vector3& ambient) { m_ambient = ambient; }
	const Vector3& GetAmbient() const { return m_ambient; }
	void SetDiffuse(const Vector3& diffuse) { m_diffuse = diffuse; }
	const Vector3& GetDiffuse() const { return m_diffuse; }
	void SetSpecular(const Vector3& specular) { m_specular = specular; }
	const Vector3& GetSpecular() const { return m_specular; }
	void SetShininess(float shininess) { m_shininess = shininess; }
	float GetShininess() const { return m_shininess; }
	void SetAlpha(float alpha) { m_alpha = alpha; }
	float GetAlpha() const { return m_alpha; }
	void SetTextureHandle(UINT tex) { m_textureHandle = tex; }
	UINT GetTextureHandle() const { return m_textureHandle; }

	void CreateBuffer(ID3D12Device* dev);
	void SetGraphicsCommand(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndexMaterial, UINT rootParameterIndexTexture);

private:
	Material() {}

};

