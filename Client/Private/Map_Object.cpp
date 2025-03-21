#include "Map_Object.h"

#include "GameInstance.h"

CMap_Object::CMap_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{ pDevice, pContext }
{
}

HRESULT CMap_Object::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMap_Object::Initialize(void* pArg)
{
    MAPOBJ_DESC* pDesc = static_cast<MAPOBJ_DESC*>(pArg);


    if (FAILED(__super::Initialize(&pDesc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    return S_OK;
}

void CMap_Object::Priority_Update(_float fTimeDelta)
{
    

}

void CMap_Object::Update(_float fTimeDelta)
{
}

void CMap_Object::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMap_Object::Render()
{
    if (FAILED(Bind_SR()))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture",
            aiTextureType_DIFFUSE, i, 0)))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CMap_Object::Ready_Components()
{
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_SHADER_MESH,
        reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

    //FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_MODEL_ROCK1,
    //    reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);
    //
    //FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_MODEL_ROCK2,
    //    reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);

    return S_OK;
}

HRESULT CMap_Object::Bind_SR()
{

    FAILED_CHECK_RETURN(m_pTransformCom->Bind_SR("g_WorldMatrix", m_pShaderCom), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, CPipeLine::D3DTS_VIEW), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, CPipeLine::D3DTS_PROJ), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4)), E_FAIL);

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
    NULL_CHECK_RETURN(pLightDesc, E_FAIL);

    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4)), E_FAIL);

    return S_OK;
}

CMap_Object* CMap_Object::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMap_Object* pInstance = new CMap_Object(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Map_Object");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMap_Object::Clone(void* pArg)
{
    CMap_Object* pInstance = new CMap_Object(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Map_Object");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMap_Object::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
