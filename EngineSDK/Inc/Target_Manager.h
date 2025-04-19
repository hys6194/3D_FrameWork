#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
private:
	CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTarget_Manager() = default;

public:
	HRESULT								Initialize();
	HRESULT								Add_RenderTarget(const _wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT								Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);

	HRESULT								Begin_MRT(const _wstring& strMRTTag);
	HRESULT								End_MRT();

#ifdef _DEBUG
public:
	HRESULT								Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT								Render(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif

public:
	ID3D11Device*								m_pDevice = { nullptr };
	ID3D11DeviceContext*						m_pContext = { nullptr };
	ID3D11RenderTargetView*						m_pBackBufferRTV = { nullptr };
	ID3D11DepthStencilView*						m_pOriginalDSV = { nullptr };
	
	// 이름으로 렌더 타겟들을 모아둠
	map<const _wstring, class CRenderTarget*>	m_mapRenderTargets;

	// 이름별로 모아둔 렌더 타겟들을 하나로 모아서 저장
	// 저장하는 이유 : 동시에 타겟들을 바인딩 할 때
	// 그 타겟들을 모아서 배열로 만들고 바인딩을 동시에 하는 작업을 하기 위해
	map <const _wstring, list<class CRenderTarget*>> m_mapMRT;



private:
	CRenderTarget*						Find_RenderTarget(const _wstring& strTargetTag);
	list<class CRenderTarget*>*			Find_MRT(const _wstring& strMRTTag);

public:
	static CTarget_Manager*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void						Free() override;
};

END