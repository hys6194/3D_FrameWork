#pragma once

#include "Base.h"

BEGIN(Engine)
class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking() = default;

public:
	HRESULT						Initialize(HWND hWnd);

	void						Copy(const _wstring& strTargetTag);
	_bool						Picking(_float3* pOut);

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	ID3D11Texture2D*			m_pTexture2D = { nullptr };
	HWND						m_hWnd = {};
	_uint						m_iViewWidth, m_iViewHeight;
	class CGameInstance*		m_pGameInstance = { nullptr };


public:
	static CPicking*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual void				Free() override;
};

END