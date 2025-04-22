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

	m_eType			= pDesc->eType;
	m_fBufferTime	= pDesc->fTime;
	m_fColor		= pDesc->fColor;
	m_fSize			= pDesc->fSize;
	m_fDir			= pDesc->fDir;

	FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);

	return S_OK;
}

void CTrail::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
}
