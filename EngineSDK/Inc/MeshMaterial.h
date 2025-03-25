#pragma once

#include "Base.h"

BEGIN(Engine)

class CMeshMaterial final : public CBase
{
private:
	CMeshMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMeshMaterial() = default;

public:
	HRESULT Initialize(const aiMaterial* pAIMaterial, const _char* pModelFilePath);
	HRESULT Initialize(const aiMaterial* _pAIMaterial, const _char* _pModelFilePath, ofstream& _OutStream);
	HRESULT Initialize(ifstream& _InStream);

	HRESULT Bind_SR(class CShader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iTextureIndex = 0);


private:
	ID3D11Device*									m_pDevice = { nullptr };
	ID3D11DeviceContext*							m_pContext = { nullptr };
	vector<ID3D11ShaderResourceView*>				m_vecMaterial[AI_TEXTURE_TYPE_MAX] = {};

public:
	static CMeshMaterial* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMaterial* pAIMaterial, const _char* pModelFilePath);
	static CMeshMaterial* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const aiMaterial* _pAIMaterial, const _char* _pModelFilePath, ofstream& _OutStream);
	static CMeshMaterial* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, ifstream& _InStream);
	virtual void Free() override;
};

END