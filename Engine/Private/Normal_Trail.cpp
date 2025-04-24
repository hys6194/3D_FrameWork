#include "Normal_Trail.h"
#include "VIBuffer_Trail.h"

#include "GameInstance.h"

CNormal_Trail::CNormal_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CTrail { pDevice , pContext }
{
}

CNormal_Trail::CNormal_Trail(const CNormal_Trail& Prototype)
    : CTrail{ Prototype }
    , m_pVIBufferCom { Prototype.m_pVIBufferCom }
{
    Safe_AddRef(m_pVIBufferCom);
}

HRESULT CNormal_Trail::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CNormal_Trail::Initialize(void* pArg)
{
    //뭐 추가적으로 넣을 거 있으면 알아서 하고

    FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);


	return S_OK;
}

void CNormal_Trail::Priority_Update(_float fTimeDelta)
{
}

void CNormal_Trail::Update(_float fTimeDelta)
{
}

void CNormal_Trail::Late_Update(_float fTimeDelta)
{
}

HRESULT CNormal_Trail::Render()
{
    FAILED_CHECK_RETURN(Bind_SR(), E_FAIL);

    if (nullptr != m_pVIBufferCom)
    {
        m_pVIBufferCom->Bind_Input_Assembler();
        m_pVIBufferCom->Render();
    }

	return S_OK;
}

HRESULT CNormal_Trail::Bind_SR()
{
    // 행렬을 던지던가 뭘하던가 해야함
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix_Ptr()), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, CPipeLine::D3DTS_VIEW), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, CPipeLine::D3DTS_PROJ), E_FAIL);

    return S_OK;
}

HRESULT CNormal_Trail::Ready_Component()
{
    FAILED_CHECK_RETURN(__super::Add_Component(m_iLevelIndex, m_strVIBufferTag,
        reinterpret_cast<CComponent**>(&m_pVIBufferCom), TEXT("Com_Model")), E_FAIL);

    FAILED_CHECK_RETURN(__super::Add_Component(m_iLevelIndex, m_strShaderTag,
        reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

    return S_OK;
}

CNormal_Trail* CNormal_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNormal_Trail* pInstance = new CNormal_Trail(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Gun_Left");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNormal_Trail::Clone(void* pArg)
{
    CNormal_Trail* pInstance = new CNormal_Trail(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Gun_Left");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNormal_Trail::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
