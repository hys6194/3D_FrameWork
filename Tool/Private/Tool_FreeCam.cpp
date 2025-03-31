 #include "Tool_FreeCam.h"

#include "GameInstance.h"
CTool_FreeCam::CTool_FreeCam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CCamera{ pDevice, pContext }
{
}

CTool_FreeCam::CTool_FreeCam(const CTool_FreeCam& Prototype)
    : CCamera{ Prototype }
{
}

HRESULT CTool_FreeCam::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTool_FreeCam::Initialize(void* pArg)
{
	CAMERA_FREE_DESC* pDesc = static_cast<CAMERA_FREE_DESC*>(pArg);

	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CTool_FreeCam::Priority_Update(_float fTimeDelta)
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

	if(GetAsyncKeyState(VK_SHIFT))
	{
		if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * m_fMouseSensor);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * m_fMouseSensor);
		}
	}

	__super::Renew_Matrices();
}

void CTool_FreeCam::Update(_float fTimeDelta)
{
	//if (m_pGameInstance->Mouse_Down(DIM_RB))
	//{
	//	CTransform::TRANSFORM_DESC Desc{};
	//
	//	Desc.fRotationPerSec = 
	//}

}

void CTool_FreeCam::Late_Update(_float fTimeDelta)
{
}

HRESULT CTool_FreeCam::Render()
{
    return S_OK;
}

HRESULT CTool_FreeCam::Ready_Components()
{
    return S_OK;
}

CTool_FreeCam* CTool_FreeCam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_FreeCam* pInstance = new CTool_FreeCam(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Tool_FreeCam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTool_FreeCam::Clone(void* pArg)
{
	CGameObject* pInstance = new CTool_FreeCam(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Tool_FreeCam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_FreeCam::Free()
{
	__super::Free();
}
