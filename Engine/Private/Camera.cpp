#include "Camera.h"
#include "Transform.h"

#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject {pDevice, pContext}
{
}

CCamera::CCamera(const CCamera& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

	m_fFov = pDesc->fFov;
	m_fAspect = pDesc->fAspect;
	m_fNear = pDesc->fNear;
	m_fFar = pDesc->fFar;

	// Transform 생성을 위해서 Initialize를 중간으로 이동
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	// 카메라의 기본 생성 위치 설정
	// float4x4의 한 행을 저장해야 하는데 float3로 하면 행렬 값이 이상해진다
	// 따라서 XMVectorSetW을 통해 float4 형식으로 바꾼다
	// 항등으로 만들어야 행렬 변환 때 의도적인 행렬 변환이 이루어 지므로 w값을 1로 세팅
	m_pTransformCom->Set_State(CTransform::STATE_POS, XMVectorSetW(XMLoadFloat3(&pDesc->vEye), 1.f));
	m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&pDesc->vAt), 1.f));
	

	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
}

void CCamera::Update(_float fTimeDelta)
{
}

void CCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

HRESULT CCamera::Renew_Matrices()
{
	_matrix		ViewMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
	_matrix		ProjMatrix = XMMatrixPerspectiveFovLH(m_fFov, m_fAspect, m_fNear, m_fFar);

	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, ViewMatrix);
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);

	return S_OK;
}

void CCamera::Free()
{
	__super::Free();
}
