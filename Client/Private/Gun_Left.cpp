#include "Gun_Left.h"
#include "GameInstance.h"

#include "Player.h"
#include "Bullet.h"

CGun_Left::CGun_Left(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CGun_Left::CGun_Left(const CGun_Left& Prototype)
    : CPartObject{ Prototype }
{
}

HRESULT CGun_Left::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGun_Left::Initialize(void* pArg)
{
    NULL_CHECK_RETURN(pArg, E_FAIL);

    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);
    m_pTargetState = pDesc->pTargetState;
    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pHandMatrix = pDesc->pHandMatrix;
    m_pOwner = pDesc->pOwner;

    m_fCool = 0.f;

    FAILED_CHECK_RETURN(__super::Initialize(pDesc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);

    return S_OK;
}

void CGun_Left::Priority_Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(CTransform::STATE_POS, XMVectorSet(19.f, -5.f, -12.5f, 1.f));

    m_fTotalTime += m_pGameInstance->Get_TimeDelta(TIME60);

    if ((CPlayer::STATE_SHOOT & *m_pTargetState) && m_fCool < m_fTotalTime)
    {
        if(E_FAIL == Create_Bullet())
            return;

        m_fCool += 0.2f;
    }

    else if ((CPlayer::STATE_SHOOT & ~*m_pTargetState))
    {
        m_fCool = 0.f;
        m_fTotalTime = 0.f;
    }
}

void CGun_Left::Update(_float fTimeDelta)
{
    _matrix matSocket = XMLoadFloat4x4(m_pHandMatrix);

    // 스케일 값에 영향을 주는 본이라면 그것을 없애주는 코드이다
    // 만약 이 코드가 없어도 잘 돌아간다면 원래 맞는 모델이라고 볼 수 있다
    //for (size_t i = 0; i < 3; i++)
    //{
    //    matSocket.r[i] = XMVector3Normalize(matSocket.r[i]);
    //}
    
    // 파츠들의 매트릭스를 부모 매트릭스에 곱하여 고정시킨다
    // 그와 동시에, Socket 매트릭스를 같이 곱하여 월드 매트릭스 상에 binding 한다
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * 
        matSocket *
        XMLoadFloat4x4(m_pParentMatrix));


}

void CGun_Left::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

}

HRESULT CGun_Left::Render()
{
    if (FAILED(Bind_SR()))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture",
            aiTextureType_DIFFUSE, i, 0)))
            return E_FAIL;

        //m_pModelCom->Bind_BoneMatrix(m_pShaderCom, "g_BoneMatrices", i);

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CGun_Left::Ready_Components()
{
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_MODEL_LGUN,
        reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_SHADER_MESH,
        reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

    return S_OK;
}

HRESULT CGun_Left::Create_Bullet()
{
    //_matrix matLocal = XMLoadFloat4x4(m_pHandMatrix);
    //
    //_float4 fTest =
    //{
    //        m_pHandMatrix->m[3][0],
    //        m_pHandMatrix->m[3][1],
    //        m_pHandMatrix->m[3][2] + 1.f,
    //        m_pHandMatrix->m[3][3],
    //};
    //
    //_float4x4 f5test;
    //XMStoreFloat4x4(&f5test,  XMLoadFloat4x4(m_pHandMatrix));
    //
    //XMStoreFloat4(f5test.m[3], fTest);

    _matrix matHand = XMMatrixMultiply(XMLoadFloat4x4(m_pHandMatrix), XMLoadFloat4x4(m_pParentMatrix));
    CBullet::BULLET_DESC Desc{};
    Desc.fSpeedPerSec = 0.5f;

    XMStoreFloat4(&Desc.fLook, m_pOwner->Get_Transform()->Get_State(CTransform::STATE_LOOK));
    XMStoreFloat4x4(&Desc.f4Hand, matHand);

    FAILED_CHECK_RETURN(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_BULLET,
        LEVEL_GAMEPLAY, TEXT("GameObject_Bullet"), &Desc), E_FAIL);

    return S_OK;
}

HRESULT CGun_Left::Bind_SR()
{
    // Combined된 월드행렬을 반환해야 한다
    // 
    //FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pParentMatrix), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix), E_FAIL);
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

CGun_Left* CGun_Left::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGun_Left* pInstance = new CGun_Left(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Gun_Left");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CGun_Left::Clone(void* pArg)
{
    CGun_Left* pInstance = new CGun_Left(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Gun_Left");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGun_Left::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
