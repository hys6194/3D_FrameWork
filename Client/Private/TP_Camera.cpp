#include "TP_Camera.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Layer.h"

CTP_Camera::CTP_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera { pDevice, pContext }
{
}

CTP_Camera::CTP_Camera(const CTP_Camera& Prototype)
	: CCamera { Prototype }
{
}

HRESULT CTP_Camera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTP_Camera::Initialize(void* pArg)
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

void CTP_Camera::Priority_Update(_float fTimeDelta)
{
	_vector vPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POS);

	//_vector vPlayerPos = XMVectorSet(vPos)
	//m_vCamEye = { 0.f, 30.f ,-50.f };
	_vector vEye = XMVectorSetW(XMLoadFloat3(&m_vCamEye), 0.f) + vPos;
	_vector vLook = vPos - vEye;

	vLook = XMVector4Normalize(vLook);
	//m_pTransformCom->LookAt(XMVectorSetW(vLook, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_POS, vEye);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVectorSetW(vLook, 0.f));
	 
	__super::Renew_Matrices();
}

void CTP_Camera::Update(_float fTimeDelta)
{

}

void CTP_Camera::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CTP_Camera::Render()
{
	

	return S_OK;
}

HRESULT CTP_Camera::Ready_Components()
{	
	return S_OK;
}

CTP_Camera* CTP_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTP_Camera* pInstance = new CTP_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : TP_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTP_Camera::Clone(void* pArg)
{
	CTP_Camera* pInstance = new CTP_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : TP_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTP_Camera::Free()
{
	__super::Free();

}
