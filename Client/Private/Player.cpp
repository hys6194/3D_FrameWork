#include "Player.h"
#include "GameInstance.h"
#include "Body_Player.h"
#include "Status.h"

#include "Ghoul.h"
#include "Moloch.h"

#include "Gun_Left.h"
#include "Gun_Right.h"

#include "StrifeState_idle.h"
#include "StrifeState_Run.h"
#include "StrifeState_Dash.h"
#include "StrifeState_Shoot.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	CContainerObject::CONTAINEROBJ_DESC	Desc{};

	lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Player"));
	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(360.f); 
	Desc.iNumPartObjects = PART_END;
	Desc.iState = STATE_IDLE;
	m_iState = Desc.iState;

	FAILED_CHECK_RETURN(__super::Initialize(&Desc), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_PartObjects(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_States(), E_FAIL);

	m_iKey = KEY_NONE;

	m_pFSMCom->Change_State(m_iState);
	

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	Input_Keys();

	m_pFSMCom->Change_State(m_iState);
	m_pFSMCom->PriUpdate_State(fTimeDelta);

	//if (m_pColliderCom->Is_Coll())
	//{
	//	//list<CGameObject*>* pObjectList = m_pGameInstance->Get_GameObjectList(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));
	//	//
	//	//for (auto* iter : *pObjectList)
	//	//{
	//	//	wstring strName = iter->Get_Name();
	//	//	CGameObject* pObject = m_pGameInstance->Find_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), strName.c_str());
	//	//
	//	//	if (Check_IncWord(strName, TEXT("Ghoul")))
	//	//	{
	//	//		CGhoul* pGhoul = static_cast<CGhoul*>(pObject);
	//	//		static_cast<CStatus*>(pGhoul->Get_Component(COM_STATUS))->Get_StatusDesc().iAttack;
	//	//	}
	//	//}
	//
	//}


	for (size_t i = 0; i < TYPE_END; i++)
	{
		if (nullptr == m_pColliderCom[i])
			continue;

		m_pColliderCom[i]->Reset();
	}

	__super::Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_pFSMCom->Update_State(fTimeDelta);

#ifdef _DEBUG
	for (size_t i = 0; i < TYPE_END; i++)
	{
		if (nullptr == m_pColliderCom[i])
			continue;


		m_pColliderCom[i]->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
	}
#endif

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pFSMCom->LateUpdate_State(fTimeDelta);

	__super::Late_Update(fTimeDelta);

	m_pTransformCom->Set_State(CTransform::STATE_POS,
		m_pNavigationCom->Compute_Height(m_pTransformCom->Get_State(CTransform::STATE_POS)));

	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

}

HRESULT CPlayer::Render()
{

#ifdef _DEBUG
	for (size_t i = 0; i < TYPE_END; i++)
	{
		if (nullptr == m_pColliderCom[i])
			continue;


		m_pColliderCom[i]->Render();
	}
#endif

	//m_fTotalTime += m_pGameInstance->Get_TimeDelta(TIME60);
	//
	//if(m_fTotalTime >= 1.f)
	//{
	//	_float4 fPos{};
	//	XMStoreFloat4(&fPos, m_pTransformCom->Get_State(CTransform::STATE_POS));
	//	TCHAR debugMessage[256];
	//	_stprintf_s(debugMessage, _T("PlayerPos: x = %.6f, y = %.6f, z = %.6f, w = %.6f\n"),
	//		fPos.x, fPos.y, fPos.z, fPos.w);
	//	OutputDebugString(debugMessage);
	//
	//	m_fTotalTime = 0.f;
	//}

	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iCellIndex = 0;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_NAVI,
		reinterpret_cast<CComponent**>(&m_pNavigationCom), COM_NAVI, &NaviDesc),E_FAIL);

	CBounding_OBB::BOUNDING_OBB_DESC		ColliderDesc{};
	ColliderDesc.vExtents		= _float3(1.f, 2.f, 1.f);
	ColliderDesc.vCenter		= _float3(0.f, ColliderDesc.vExtents.y, 0.f);
	ColliderDesc.strCollTag		= Get_Name() + TEXT("_Body");
	ColliderDesc.iOption		= COLL_OPT::OP_TARGET;
	ColliderDesc.eType			= TYPE::TYPE_OBB;
	ColliderDesc.pOwner			= this;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_OBB,
		reinterpret_cast<CComponent**>(&m_pColliderCom[COLL_OBB]), COM_COLL_OBB, &ColliderDesc), E_FAIL);

	CBounding_Sphere::BOUNDING_SPHERE_DESC		SphereDesc{};
	SphereDesc.fRadius = 5.f;
	SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);
	SphereDesc.strCollTag = Get_Name() + TEXT("_Body_Detect");
	SphereDesc.iOption = COLL_OPT::OP_DETECT;
	SphereDesc.eType = TYPE::TYPE_SPHERE;
	SphereDesc.pOwner = this;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_SPHERE,
		reinterpret_cast<CComponent**>(&m_pColliderCom[COLL_SPHERE]), COM_COLL_SPHERE, &SphereDesc), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_FSM,
		reinterpret_cast<CComponent**>(&m_pFSMCom), COM_FSM), E_FAIL);

	CStatus::STATUS_DESC StatusDesc{};
	StatusDesc.iAttack = 5;
	StatusDesc.iHP = 200;
	StatusDesc.pOwner = this;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_STATUS,
		reinterpret_cast<CComponent**>(&m_pStatusCom), COM_STATUS, &StatusDesc), E_FAIL);

	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	// Body
	CBody_Player::BODY_PLAYER_DESC		BodyDesc{};
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.pTargetState = &m_iState;
	
	FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY, PRO_OBJ_BODY, PART_BODY, &BodyDesc), E_FAIL);

	// Sword
	//Weapon::WEAPON_DESC  WDesc{};
	//// WDesc.pSocketMatrix = 바디플레이어에 있는 특정 뼈(손)의 매트릭스를 가져와야 함 -> 바디 플레이어에서 특정 뼈를 가져오는 작업을 해야함
	//WDesc.pSocketMatrix = dynamic_cast<Body_Player*>(m_vecParts[PART_BODY])->Get_f4SocketMatrix(TEXT("Socket_Weapon"));
	//WDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	//WDesc.pTargetState = &m_iState;
	//
	//FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY, PRO_OBJ_WEAPON, PART_WEAPON, &WDesc), E_FAIL);

	CGun_Left::WEAPON_DESC  GDesc1{};
	// WDesc.pSocketMatrix = 바디플레이어에 있는 특정 뼈(손)의 매트릭스를 가져와야 함 -> 바디 플레이어에서 특정 뼈를 가져오는 작업을 해야함
	GDesc1.pSocketMatrix	= dynamic_cast<CBody_Player*>(m_vecParts[PART_BODY])->Get_f4SocketMatrix(SOCKET_HOLSTER_LEFT);
	GDesc1.pHandMatrix		= dynamic_cast<CBody_Player*>(m_vecParts[PART_BODY])->Get_f4SocketMatrix(SOCKET_LEFT_HAND);
	GDesc1.pParentMatrix	= m_pTransformCom->Get_WorldMatrix_Ptr();
	GDesc1.pTargetState		= &m_iState;
	GDesc1.pOwner = this;

	FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY, PRO_OBJ_L_GUN, PART_LGUN, &GDesc1), E_FAIL);

	CGun_Right::WEAPON_DESC  GDesc2{};
	// WDesc.pSocketMatrix = 바디플레이어에 있는 특정 뼈(손)의 매트릭스를 가져와야 함 -> 바디 플레이어에서 특정 뼈를 가져오는 작업을 해야함
	GDesc2.pSocketMatrix	= dynamic_cast<CBody_Player*>(m_vecParts[PART_BODY])->Get_f4SocketMatrix(SOCKET_HOLSTER_RIGHT);
	GDesc2.pHandMatrix		= dynamic_cast<CBody_Player*>(m_vecParts[PART_BODY])->Get_f4SocketMatrix(SOCKET_RIGHT_HAND);
	GDesc2.pParentMatrix	= m_pTransformCom->Get_WorldMatrix_Ptr();
	GDesc2.pTargetState		= &m_iState;
	GDesc2.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY, PRO_OBJ_R_GUN, PART_RGUN, &GDesc2), E_FAIL);


	

	return S_OK;
}

HRESULT CPlayer::Ready_States()
{
	CState* pState;

	pState = CStrifeState_Idle::Create(this ,m_vecParts[PART_BODY]);
	m_pFSMCom->Add_State(CPlayer::STATE_IDLE, pState);

	pState = CStrifeState_Run::Create(this, m_vecParts[PART_BODY]);
	m_pFSMCom->Add_State(CPlayer::STATE_RUN, pState);

	pState = CStrifeState_Dash::Create(this, m_vecParts[PART_BODY]);
	m_pFSMCom->Add_State(CPlayer::STATE_DASH, pState);

	pState = CStrifeState_Shoot::Create(this, m_vecParts[PART_BODY]);
	m_pFSMCom->Add_State(CPlayer::STATE_SHOOT, pState);

	return S_OK;
}

HRESULT CPlayer::Bind_SR()
{
	
	return S_OK;
}

void CPlayer::Input_Keys()
{
	if (m_pGameInstance->Key_Pressing(DIK_S))
	{
		m_iKey |= KEY_DOWN;
	}
	else if (!m_pGameInstance->Key_Pressing(DIK_S))
	{
		m_iKey &= ~KEY_DOWN;
	}

	if (m_pGameInstance->Key_Pressing(DIK_W))
	{
		m_iKey |= KEY_UP;
	}
	else if (!m_pGameInstance->Key_Pressing(DIK_W))
	{
		m_iKey &= ~KEY_UP;
	}

	if (m_pGameInstance->Key_Pressing(DIK_A))
	{
		m_iKey |= KEY_LEFT;
	}
	else if (!m_pGameInstance->Key_Pressing(DIK_A))
	{
		m_iKey &= ~KEY_LEFT;
	}

	if (m_pGameInstance->Key_Pressing(DIK_D))
	{
		m_iKey |= KEY_RIGHT;
	}
	else if (!m_pGameInstance->Key_Pressing(DIK_D))
	{
		m_iKey &= ~KEY_RIGHT;
	}

	if (m_pGameInstance->Key_Down(DIK_LSHIFT))
	{
		m_iKey |= KEY_SHIFT;
	}
	else if (!m_pGameInstance->Key_Down(DIK_LSHIFT))
	{
		m_iKey &= ~KEY_SHIFT;
	}

	if (m_pGameInstance->Get_DIMouseState(DIM_LB))
	{
		m_iKey |= KEY_LB;
	}
	else if (!m_pGameInstance->Get_DIMouseState(DIM_LB))
	{
		m_iKey &= ~KEY_LB;
	}

}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	//Safe_Release(m_pColliderCom);
	Safe_Release(m_pFSMCom);
	Safe_Release(m_pStatusCom);


#ifdef _DEBUG
	for (size_t i = 0; i < TYPE_END; i++)
	{
		Safe_Release(m_pColliderCom[i]);
	}

#endif

}
