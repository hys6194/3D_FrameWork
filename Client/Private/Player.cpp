#include "Player.h"
#include "GameInstance.h"
#include "ContainerObject.h"
#include "Body_Player.h"
#include "Weapon.h"

Player::Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: ContainerObject{ pDevice, pContext }
{
}

Player::Player(const Player& Prototype)
	: ContainerObject{ Prototype }
{
}

HRESULT Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Player::Initialize(void* pArg)
{
	ContainerObject::CONTAINEROBJ_DESC	Desc{};

	lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Player"));
	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f); 
	Desc.iNumPartObjects = PART_END;

	FAILED_CHECK_RETURN(__super::Initialize(&Desc), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_PartObjects(), E_FAIL);


	return S_OK;
}

void Player::Priority_Update(_float fTimeDelta)
{
	_vector vLook = m_pTransformCom->Get_State(Transform::STATE_LOOK);
	vLook = XMVector4Normalize(vLook);

	
	if( (GetAsyncKeyState(VK_DOWN) & 0x8000) ||	(GetAsyncKeyState(VK_LEFT) & 0x8000) ||	(GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000))
	{
		if (m_iState & STATE_IDLE)
			m_iState ^= STATE_IDLE;
		m_iState |= STATE_RUN;

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			m_pTransformCom->Rotation(AXIS_Y, XMConvertToRadians(180.f));
			m_pTransformCom->Go_Straight(fTimeDelta);
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			m_pTransformCom->Rotation(AXIS_Y, XMConvertToRadians(-90.f));
			m_pTransformCom->Go_Straight(fTimeDelta);
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			m_pTransformCom->Rotation(AXIS_Y, XMConvertToRadians(90.f));
			m_pTransformCom->Go_Straight(fTimeDelta);
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			m_pTransformCom->Rotation(AXIS_Y, XMConvertToRadians(0.f));
			m_pTransformCom->Go_Straight(fTimeDelta);
		}
	}



	//// Test
	//if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	//{
	//}
	
	else
	{
		m_iState = STATE_IDLE;
	}

	__super::Priority_Update(fTimeDelta);
}

void Player::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void Player::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT Player::Render()
{
	return S_OK;
}

HRESULT Player::Ready_Components()
{


	return S_OK;
}

HRESULT Player::Ready_PartObjects()
{
	// Body
	Body_Player::BODY_PLAYER_DESC		BodyDesc{};
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.pTargetState = &m_iState;

	FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY,TEXT("Prototype_GameObject_Player_Body"), PART_BODY, &BodyDesc), E_FAIL);

	// Sword
	Weapon::WEAPON_DESC  WDesc{};
	// WDesc.pSocketMatrix = 바디플레이어에 있는 특정 뼈(손)의 매트릭스를 가져와야 함 -> 바디 플레이어에서 특정 뼈를 가져오는 작업을 해야함
	WDesc.pSocketMatrix = dynamic_cast<Body_Player*>(m_vecParts[PART_BODY])->Get_f4SocketMatrix(TEXT("Socket_Weapon"));
	WDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	WDesc.pTargetState = &m_iState;


	//FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Weapon"), PART_WEAPON, &WDesc), E_FAIL);

	return S_OK;
}


HRESULT Player::Bind_SR()
{
	
	return S_OK;
}

Player* Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Player* pInstance = new Player(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

GameObject* Player::Clone(void* pArg)
{
	Player* pInstance = new Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Player::Free()
{
	__super::Free();
}
