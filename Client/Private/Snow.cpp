#include "Snow.h"
#include "GameInstance.h"

CSnow::CSnow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject { pDevice, pContext }
{
}

CSnow::CSnow(const CSnow& Prototype)
    : CGameObject { Prototype }
{
}

HRESULT CSnow::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSnow::Initialize(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC Desc{};

    lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Snow"));
    Desc.fSpeedPerSec = 0.f;
    Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Component()))
        return E_FAIL;

    return S_OK;
}

void CSnow::Priority_Update(_float fTimeDelta)
{
}

void CSnow::Update(_float fTimeDelta)
{
    m_pVIBufferCom->Drop(fTimeDelta);
}

void CSnow::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CSnow::Render()
{
    if (FAILED(Bind_SR()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_Input_Assembler();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CSnow::Ready_Component()
{
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Particle_Snow"),
        reinterpret_cast<CComponent**>(&m_pVIBufferCom), TEXT("Com_VIBuffer")), E_FAIL);

    /* Com_Shader */
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxRectParticle"),
        reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")),E_FAIL) ;

    /* Com_Texture */
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
        reinterpret_cast<CComponent**>(&m_pTextureCom), TEXT("Com_Texture")), E_FAIL);

    return S_OK;
}

HRESULT CSnow::Bind_SR()
{
    FAILED_CHECK_RETURN(m_pTransformCom->Bind_SR("g_WorldMatrix", m_pShaderCom), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, CPipeLine::D3DTS_VIEW), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, CPipeLine::D3DTS_PROJ), E_FAIL);
    FAILED_CHECK_RETURN(m_pTextureCom->Bind_SR("g_DiffuseTexture", m_pShaderCom, 0), E_FAIL);


    return S_OK;
}

CSnow* CSnow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSnow* pInstance = new CSnow(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CSnow");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSnow::Clone(void* pArg)
{
    CSnow* pInstance = new CSnow(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CSnow");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSnow::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);


}
