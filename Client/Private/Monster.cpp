#include "Monster.h"

#include "GameInstance.h"
#include "Body_Ghoul.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject{ pDevice , pContext }
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CContainerObject{ Prototype }
	, m_iIndex { Prototype.m_iIndex }
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	CONTAINEROBJ_DESC* pDesc = static_cast<CONTAINEROBJ_DESC*>(pArg);

	pDesc->iNumPartObjects = PART_END;
	lstrcpy(pDesc->szGameObjectTag, TEXT("GameObject_Monster"));

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	m_pFSMCom->PriUpdate_State(fTimeDelta);
	m_pFSMCom->Change_State(m_iState);

	m_pColliderCom->Reset();

	__super::Priority_Update(fTimeDelta);
}

void CMonster::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_pFSMCom->Update_State(fTimeDelta);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
}


void CMonster::Late_Update(_float fTimeDelta)
{
	m_pFSMCom->LateUpdate_State(fTimeDelta);

	__super::Late_Update(fTimeDelta);

	m_pTransformCom->Set_State(CTransform::STATE_POS,
		m_pNavigationCom->Compute_Height(m_pTransformCom->Get_State(CTransform::STATE_POS)));

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif 

	if (FAILED(Bind_SR()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Ready_Components()
{
	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iCellIndex = 0;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_NAVI,
		reinterpret_cast<CComponent**>(&m_pNavigationCom), COM_NAVI, &NaviDesc), E_FAIL);


	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_FSM,
		reinterpret_cast<CComponent**>(&m_pFSMCom), COM_FSM), E_FAIL);

	return S_OK;
}

HRESULT CMonster::Bind_SR()
{
	return S_OK;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pFSMCom);
}
