#include "TP_Camera.h"

#include "GameInstance.h"
#include "Layer.h"

TP_Camera::TP_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Camera { pDevice, pContext }
{
}

TP_Camera::TP_Camera(const TP_Camera& Prototype)
	: Camera { Prototype }
{
}

HRESULT TP_Camera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT TP_Camera::Initialize(void* pArg)
{
	TP_CAMERA_DESC* pDesc = static_cast<TP_CAMERA_DESC*>(pArg);

	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;	

	Layer* pLayer = m_pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("GameObject_Player"));

	pLayer->Get_Object(TEXT("GameObject_Player"));


	return S_OK;
}

void TP_Camera::Priority_Update(_float fTimeDelta)
{
	//if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	//{
	//	m_pTransformCom->Go_Straight(fTimeDelta);
	//}
	//if (GetKeyState('S') & 0x8000)
	//{
	//	m_pTransformCom->Go_Backward(fTimeDelta);
	//}
	//if (GetKeyState('A') & 0x8000)
	//{
	//	m_pTransformCom->Go_Left(fTimeDelta);
	//}
	//if (GetKeyState('D') & 0x8000)
	//{
	//	m_pTransformCom->Go_Right(fTimeDelta);
	//}
	//
	//_long		MouseMove{};
	//
	//if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X))
	//{
	//	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * m_fMouseSensor);
	//}
	//
	//if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_Y))
	//{
	//	m_pTransformCom->Turn(m_pTransformCom->Get_State(Transform::STATE_RIGHT), fTimeDelta * MouseMove * m_fMouseSensor);
	//}

	// Todo : 플레이어의 위치를 가져와서 카메라의 위치 + 플레이어 위치 해야 함
	// 카메라가 바라보는 방향은 플레이어의 위치로 한다

	__super::Renew_Matrices();
}

void TP_Camera::Update(_float fTimeDelta)
{
	_vector vecEye = XMVectorSetW(XMLoadFloat3(&m_vCamEye), 1.f);


	//m_pTransformCom->Set_State(Transform::STATE_POS, )
}

void TP_Camera::Late_Update(_float fTimeDelta)
{
	
}

HRESULT TP_Camera::Render()
{
	

	return S_OK;
}

HRESULT TP_Camera::Ready_Components()
{
	Layer* pLayer = m_pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("GameObject_Player"));

	pLayer->
	
	return S_OK;
}

TP_Camera* TP_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	TP_Camera* pInstance = new TP_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : TP_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

GameObject* TP_Camera::Clone(void* pArg)
{
	TP_Camera* pInstance = new TP_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : TP_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void TP_Camera::Free()
{
	__super::Free();

}
