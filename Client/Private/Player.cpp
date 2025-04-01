#include "Player.h"
#include "GameInstance.h"
#include "Body_Player.h"

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
	Desc.fRotationPerSec = XMConvertToRadians(90.f); 
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

	m_pColliderCom->Reset();

	__super::Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_pFSMCom->Update_State(fTimeDelta);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

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
	m_pColliderCom->Render();
#endif

	_float4 fPos{};
	XMStoreFloat4(&fPos, m_pTransformCom->Get_State(CTransform::STATE_POS));
	TCHAR debugMessage[256];
	_stprintf_s(debugMessage, _T("Debug_Value: x = %.6f, y = %.6f, z = %.6f, w = %.6f\n"), 
		fPos.x, fPos.y, fPos.z, fPos.w);
	OutputDebugString(debugMessage);

	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iCellIndex = 0;

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_NAVI,
		reinterpret_cast<CComponent**>(&m_pNavigationCom), COM_NAVI, &NaviDesc),E_FAIL);

	CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc{};
	ColliderDesc.vExtents = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vCenter  = _float3(0.f, ColliderDesc.vExtents.y, 0.f);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_AABB,
		reinterpret_cast<CComponent**>(&m_pColliderCom), COM_COLL_AABB, &ColliderDesc), E_FAIL);


	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_FSM,
		reinterpret_cast<CComponent**>(&m_pFSMCom), COM_FSM), E_FAIL);


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

	pState = CStrifeState_Idle::Create(m_pDevice, m_pContext,this ,m_vecParts[PART_BODY]);
	m_pFSMCom->Add_State(CPlayer::STATE_IDLE, pState);

	pState = CStrifeState_Run::Create(m_pDevice, m_pContext, this, m_vecParts[PART_BODY]);
	m_pFSMCom->Add_State(CPlayer::STATE_RUN, pState);

	pState = CStrifeState_Dash::Create(m_pDevice, m_pContext, this, m_vecParts[PART_BODY]);
	m_pFSMCom->Add_State(CPlayer::STATE_DASH, pState);

	pState = CStrifeState_Shoot::Create(m_pDevice, m_pContext, this, m_vecParts[PART_BODY]);
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
	
	if (GetKeyState('0') & 0x8000)
	{
		Get_Transform()->Set_State(CTransform::STATE_POS, XMVectorSet(0.f,0.f,0.f,1.f));
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
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pFSMCom);

}
