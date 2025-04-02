#include "Monster.h"

#include "GameInstance.h"
#include "Body_Monster.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject{ pDevice , pContext }
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC			Desc{};

	lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Monster"));
	Desc.fSpeedPerSec = 0.f;
	Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POS,
	//	XMVectorSet(m_pGameInstance->Random(0, 10), 1.f,
	//		m_pGameInstance->Random(0, 10), 1.f));


	m_pFSMCom->Change_State(m_iState);

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	m_pFSMCom->Change_State(m_iState);
	m_pFSMCom->PriUpdate_State(fTimeDelta);

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
HRESULT CMonster::Ready_PartObjects()
{
	return S_OK;
}
HRESULT CMonster::Ready_Components()
{
	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iCellIndex = 0;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_NAVI,
		reinterpret_cast<CComponent**>(&m_pNavigationCom), COM_NAVI, &NaviDesc), E_FAIL);

	CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc{};
	ColliderDesc.vExtents = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_AABB,
		reinterpret_cast<CComponent**>(&m_pColliderCom), COM_COLL_AABB, &ColliderDesc), E_FAIL);


	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_FSM,
		reinterpret_cast<CComponent**>(&m_pFSMCom), COM_FSM), E_FAIL);

	return S_OK;
}


HRESULT CMonster::Ready_States()
{
	return S_OK;
}

HRESULT CMonster::Bind_SR()
{
	return S_OK;
}

CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster* pInstance = new CMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Monster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
