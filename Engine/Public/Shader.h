#pragma once

#include "Component.h"

BEGIN (Engine)

class ENGINE_DLL Shader final : public Component
{
private:
	Shader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Shader(const Shader& Prototype);
	virtual ~Shader() = default; 

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg) override;

public:
	// hlsl 파일에 Matrix 및 Texture2D를 전달하기 위해서 만든 함수
	HRESULT Begin(_uint iPassIndex);
	HRESULT Apply_Matirx(const _char* pConstantName, const _float4x4* pMatrix);
	HRESULT Apply_ShaderResourceView(const _char* pConstantName, ID3D11ShaderResourceView* pSRV);

private:
	ID3DX11Effect*				m_pEffect = { nullptr };
	vector<ID3D11InputLayout*>  m_vecInputLayOut;

	_uint						m_iNumPasses = { };

public:
	static Shader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;
};

END