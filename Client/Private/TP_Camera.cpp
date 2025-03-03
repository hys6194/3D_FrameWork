#include "TP_Camera.h"

#include "GameInstance.h"
#include "GameObject.h"
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

	m_pPlayer = m_pGameInstance->Find_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("GameObject_Player"));

	return S_OK;
}

void TP_Camera::Priority_Update(_float fTimeDelta)
{
	_vector vPos = m_pPlayer->Get_Transform()->Get_State(Transform::STATE_POS);
	_vector vEye = XMVectorSetW(XMLoadFloat3(&m_vCamEye), 0.f) + vPos;
	_vector vLook = vPos - vEye;

	vLook = XMVector4Normalize(vLook);


	m_pTransformCom->Set_State(Transform::STATE_POS, vEye);
	m_pTransformCom->Set_State(Transform::STATE_LOOK, vLook);
	//m_pTransformCom->LookAt(vPos);

	__super::Renew_Matrices();
}

void TP_Camera::Update(_float fTimeDelta)
{

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
