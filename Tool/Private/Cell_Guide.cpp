#include "Cell_Guide.h"
#include "GameInstance.h"
#include "VIBuffer_Cell.h"
// 일단 피킹하고 삼각형을 그리는 흐름을 생각해보자
// Navi 클릭하고 메쉬에 제대로 피킹이 되었다면 이 클래스를 생성하여
// 삼각형을 미리 그리고 Cell의 행렬 정보를 완벽하게 만들었다면
// Render를 끄는 방식으로 만들자
//
//아니 진짜 이걸 클론해야하는게 맞냐 진짜로????
//그러면 base로 상속받고 GameObject를 따로 관리하면서 Transform을 선언하던가 해야할 듯한데?
//근데 Transform이 필요한가?
//애초에 삼각형만 그려내는 기능과 위치를 그에 맞게 그려내야하는 거 아닌가
//그러면 필요 없어 보이는데 Base로 바꾸자

CCell_Guide::CCell_Guide(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
    , m_pContext { pContext}
    , m_pGameInstance {CGameInstance::GetInstance()}
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CCell_Guide::Initialize()
{
    // 이니셜라이즈 할때 뭘 해야할까?
    // 포인트를 0,1,2 순서로 먼저 만들까

    XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());

    FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);



    return S_OK;
}

void CCell_Guide::Update(_float fTimeDelta)
{
    // 여기서 정점들의 위치를 선언하면 matrix 값을 직접 수정해야 함
}

HRESULT CCell_Guide::Render()
{
    FAILED_CHECK_RETURN(Bind_SR(), E_FAIL);

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_Input_Assembler();

    m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CCell_Guide::Bind_SR()
{
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_matWorld), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, CPipeLine::D3DTS_VIEW), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, CPipeLine::D3DTS_PROJ), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4)), E_FAIL);

	return S_OK;
}

HRESULT CCell_Guide::Ready_Component()
{
    m_pShaderCom = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"),
        VTXPOS::ElementDesc, VTXPOS::iNumElements);

    Safe_AddRef(m_pShaderCom);

	return S_OK;
}

void CCell_Guide::Check_Cell_Translation()
{
}

CCell_Guide* CCell_Guide::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCell_Guide* pInstance = new CCell_Guide(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed To Created : Navi_Cell");
        Safe_Release(pInstance);
    }

    return pInstance;
}

//CGameObject* CCell_Guide::Clone(void* pArg)
//{
//    CCell_Guide* pInstance = new CCell_Guide(*this);
//
//
//    // GameObject라서 좀 불합리한데 귀찮다 그냥하자
//    if (FAILED(pInstance->Initialize(pArg)))
//    {
//        MSG_BOX("Failed To Cloned : Navi_Cell");
//    }
//        Safe_Release(pInstance);
//
//    return pInstance;
//} 

void CCell_Guide::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);
}
