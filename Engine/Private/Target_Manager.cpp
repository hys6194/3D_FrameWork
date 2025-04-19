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
	return S_OK;
}

HRESULT CTarget_Manager::End_MRT()
{
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
