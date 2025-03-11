#pragma once

#include "Base.h"

BEGIN(Engine)

class MeshMaterial final : public CBase
{
private:
	MeshMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~MeshMaterial() = default;

public:
	HRESULT Initialize(const aiMaterial* pAIMaterial, const _char* pModelFilePath);
	HRESULT Bind_SR(class Shader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iTextureIndex = 0);
private:
	ID3D11Device*									m_pDevice = { nullptr };
	ID3D11DeviceContext*							m_pContext = { nullptr };
	vector<ID3D11ShaderResourceView*>				m_vecMaterial[AI_TEXTURE_TYPE_MAX] = {};

public:
	static MeshMaterial* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMaterial* pAIMaterial, const _char* pModelFilePath);
	virtual void Free() override;
};

END