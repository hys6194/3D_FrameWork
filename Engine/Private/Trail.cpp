#include "Trail.h"
#include "GameInstance.h"

CTrail::CTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice ,pContext }
{
}

CTrail::CTrail(const CTrail& Prototype)
	: CGameObject{ Prototype }
	, m_pShaderCom { Prototype.m_pShaderCom }
{
	Safe_AddRef(m_pShaderCom);
}

HRESULT CTrail::Initialize(void* pArg)
{
	CTrail::TRAIL_DESC* pDesc = static_cast<CTrail::TRAIL_DESC*>(pArg);

	m_fDir			 = pDesc->fDir;	// 근데 이거 필요하긴 할까? Trail을 바라봐야 하는게 필요한가? 깊이 고민 좀 해보자
	m_fPos			 = pDesc->fPos;
	m_eType			 = pDesc->eType;
	m_fBufferTime	 = pDesc->fTime;
	m_fSize			 = pDesc->fSize;
	m_fColor		 = pDesc->fColor;
	m_pOwner		 = pDesc->pObject;
	m_iLevelIndex	 = pDesc->iLevelIndex;
	m_strShaderTag   = pDesc->strShaderTag;
	m_strVIBufferTag = pDesc->strVIBufferTag;

	FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);

	return S_OK;
}

void CTrail::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
}
