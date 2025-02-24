#include "Camera_Free.h"

#include "GameInstance.h"

Camera_Free::Camera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Camera { pDevice, pContext }
{
}

Camera_Free::Camera_Free(const Camera_Free& Prototype)
	: Camera { Prototype }
{
}

HRESULT Camera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Camera_Free::Initialize(void* pArg)
{
	CAMERA_FREE_DESC* pDesc = static_cast<CAMERA_FREE_DESC*>(pArg);

	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;	

	return S_OK;
}

void Camera_Free::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (GetKeyState('A') & 0x8000)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	_long		MouseMove{};

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * m_fMouseSensor);
	}

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(Transform::STATE_RIGHT), fTimeDelta * MouseMove * m_fMouseSensor);
	}

	__super::Renew_Matrices();
}

void Camera_Free::Update(_float fTimeDelta)
{

}

void Camera_Free::Late_Update(_float fTimeDelta)
{
	
}

HRESULT Camera_Free::Render()
{
	

	return S_OK;
}

HRESULT Camera_Free::Ready_Components()
{

	
	return S_OK;
}

Camera_Free* Camera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Camera_Free* pInstance = new Camera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Camera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

GameObject* Camera_Free::Clone(void* pArg)
{
	Camera_Free* pInstance = new Camera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Camera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Camera_Free::Free()
{
	__super::Free();

}
