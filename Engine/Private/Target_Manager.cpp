#include "Target_Manager.h"
#include "RenderTarget.h"

CTarget_Manager::CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CTarget_Manager::Initialize()
{
    return S_OK;
}

HRESULT CTarget_Manager::Add_RenderTarget(const _wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	if (nullptr != Find_RenderTarget(strTargetTag))
		return E_FAIL;

	CRenderTarget* pRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext, iSizeX, iSizeY, ePixelFormat, vClearColor);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_mapRenderTargets.emplace(strTargetTag, pRenderTarget);

    return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>	RenderTargets;

		RenderTargets.push_back(pRenderTarget);

		m_mapMRT.emplace(strMRTTag, RenderTargets);
	}
	else
	{
		pMRTList->push_back(pRenderTarget);
	}

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(const _wstring& strMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	// 그래픽 디바이스에 있던 백버퍼와 깊이버퍼를 가져옴
	m_pContext->OMGetRenderTargets(1, &m_pBackBufferRTV, &m_pOriginalDSV);

	ID3D11RenderTargetView* RenderTargets[8] = {};

	_uint		iNumRenderTargets = { 0 };

	for (auto& pRenderTarget : *pMRTList)
	{
		RenderTargets[iNumRenderTargets++] = pRenderTarget->Get_RTV();
	}

	m_pContext->OMSetRenderTargets(iNumRenderTargets, RenderTargets, m_pOriginalDSV);

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT()
{
	// 렌더타겟을 세팅하게 되면 자동적으로 레퍼런스 카운터가 증가하는 구조임
	m_pContext->OMSetRenderTargets(1, &m_pBackBufferRTV, m_pOriginalDSV);

	// 따라서 세팅한 후 바로 레퍼런스 카운터를 관리
	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pOriginalDSV);

	return S_OK;
}

HRESULT CTarget_Manager::Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	for (auto& pRenderTarget : *pMRTList)
		pRenderTarget->Render(pShader, pVIBuffer);

	return S_OK;
}

CRenderTarget* CTarget_Manager::Find_RenderTarget(const _wstring& strTargetTag)
{
	auto	iter = m_mapRenderTargets.find(strTargetTag);

	if (iter == m_mapRenderTargets.end())
		return nullptr;

	return iter->second;
}

list<class CRenderTarget*>* CTarget_Manager::Find_MRT(const _wstring& strMRTTag)
{
	auto	iter = m_mapMRT.find(strMRTTag);

	if (iter == m_mapMRT.end())
		return nullptr;

	return &iter->second;
}

CTarget_Manager* CTarget_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTarget_Manager* pInstance = new CTarget_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CTarget_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTarget_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_mapMRT)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);
		Pair.second.clear();
	}
	m_mapMRT.clear();

	for (auto& Pair : m_mapRenderTargets)
		Safe_Release(Pair.second);
	m_mapRenderTargets.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
