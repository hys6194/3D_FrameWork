#include "Monster.h"
#include "Player.h"
#include "Status.h"

#include "GameInstance.h"
#include "Body_Monster.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject{ pDevice , pContext }
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CContainerObject{ Prototype }
	, m_iIndex { Prototype.m_iIndex }
	, m_iState { Prototype.m_iState }
	, m_iPreState { Prototype.m_iPreState }
	, m_pFSMCom{ Prototype.m_pFSMCom }
	, m_pNavigationCom { Prototype.m_pNavigationCom }
	, m_pColliderCom { Prototype.m_pColliderCom }
	, m_pStatusCom { Prototype.m_pStatusCom }
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	CONTAINEROBJ_DESC* pDesc = static_cast<CONTAINEROBJ_DESC*>(pArg);
	MONSTER_DESC* pMonsterDesc = static_cast<MONSTER_DESC*>(pArg);

	_wstring strMonsterTag = TEXT("GameObject_Monster") + pMonsterDesc->strMonsterName;
	pDesc->iNumPartObjects = PART_END;
	lstrcpy(pDesc->szGameObjectTag, strMonsterTag.c_str());

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	if (0 >= m_pStatusCom->Get_StatusDesc().iHP)
	{
		m_pGameInstance->Secede_Update(m_pColliderCom->Get_Bounder());
		m_iState = STATE_DEAD;
		m_bIsDead = true;
		m_bRec = true;
	}

	if (m_pColliderCom->Is_Coll())
	{
		m_bHit = true;

		CGameObject* pObject = m_pGameInstance->Find_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("GameObject_Player"));
		CPlayer* pPlayer = static_cast<CPlayer*>(pObject);

		CStatus* pPlayerStatus = static_cast<CStatus*>(pPlayer->Get_Component(COM_STATUS));

		m_pStatusCom->Take_Damage(pPlayerStatus->Get_StatusDesc().iAttack);
	}
	else
		m_bHit = false;

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
	if (m_bIsDead)
		return E_ABORT;

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
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pFSMCom);
}
