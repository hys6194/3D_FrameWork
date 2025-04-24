#include "Monster.h"
#include "Player.h"
#include "Status.h"
#include "Attack.h"

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
	, m_bIsBoss { Prototype.m_bIsBoss }
	, m_iPreState { Prototype.m_iPreState }
	, m_pFSMCom{ Prototype.m_pFSMCom }
	, m_pNavigationCom { Prototype.m_pNavigationCom }
	, m_pColliderCom { Prototype.m_pColliderCom[COLL_END] }
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
		m_pGameInstance->Secede_Update(m_pColliderCom[COLL_AABB]->Get_Bounder());
		m_pGameInstance->Secede_Update(m_pColliderCom[COLL_SPHERE]->Get_Bounder());
		m_iState = STATE_DEAD;
		m_bRec = false;

		// 여기에서 피가 0이 되면 터지는 파티클이 연출되어야 한다

	}

	// 이 부분을 따로 빼서 적용한다던가
	// 아니면 aabb 부분만 체크하게 하자
	if (m_pColliderCom[COLL_AABB]->Is_Coll())
	{
		m_bHit = true;
		m_bRec = true;
		CGameObject* pObject = m_pGameInstance->Find_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("GameObject_Player"));
		CPlayer* pPlayer = static_cast<CPlayer*>(pObject);

		CStatus* pPlayerStatus = static_cast<CStatus*>(pPlayer->Get_Component(COM_STATUS));

		m_pStatusCom->Take_Damage(pPlayerStatus->Get_StatusDesc().iAttack);
	}

	else
		m_bHit = false;

	m_pFSMCom->PriUpdate_State(fTimeDelta);
	m_pFSMCom->Change_State(m_iState);

	for (size_t i = 0; i < TYPE_END; i++)
	{
		if (nullptr == m_pColliderCom[i])
			continue;

		m_pColliderCom[i]->Reset();
	}

	m_fTotalTime += fTimeDelta;

	// 공격 패턴에 넘어갈 수 있는 조건
	if (m_fTotalTime >= m_fAttackCoolTime)
	{
		m_fTotalTime = 0.f;
		m_bAttack = true;
	}

	__super::Priority_Update(fTimeDelta);
}

void CMonster::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_pFSMCom->Update_State(fTimeDelta);

	m_pAttackCom->Update_CoolTime(fTimeDelta);


#ifdef _DEBUG
	for (size_t i = 0; i < TYPE_END; i++)
	{
		if (nullptr == m_pColliderCom[i])
			continue;

		m_pColliderCom[i]->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
	}
#endif
}


void CMonster::Late_Update(_float fTimeDelta)
{
	m_pFSMCom->LateUpdate_State(fTimeDelta);

	__super::Late_Update(fTimeDelta);

	m_pTransformCom->Set_State(CTransform::STATE_POS,
		m_pNavigationCom->Compute_Height(m_pTransformCom->Get_State(CTransform::STATE_POS)));

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

	if (m_bIsDead)
		return;

#ifdef _DEBUG
	for (size_t i = 0; i < TYPE_END; i++)
	{
		m_pGameInstance->Add_Renderer_DebugComponent(m_pColliderCom[i]);
	}
	m_pGameInstance->Add_Renderer_DebugComponent(m_pNavigationCom);

#endif
}

HRESULT CMonster::Render()
{
	if (m_bIsDead)
		return E_ABORT;

	if (FAILED(Bind_SR()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Ready_UI_HP()
{
	return E_NOTIMPL;
}

HRESULT CMonster::Ready_Components()
{
	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iCellIndex = 0;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_ATTACK,
		reinterpret_cast<CComponent**>(&m_pAttackCom), COM_ATTACK), E_FAIL);

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
	Safe_Release(m_pAttackCom);

}
