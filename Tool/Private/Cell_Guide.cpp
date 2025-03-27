#include "Cell_Guide.h"
#include "GameInstance.h"
#include "Navi_Cell.h"
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
    : CGameObject{ pDevice, pContext }
{

}

CCell_Guide::CCell_Guide(const CCell_Guide& Prototype)
    : CGameObject { Prototype }
    , m_matWorld { Prototype.m_matWorld }
    , m_bIsModify { Prototype.m_bIsModify }
{
}

HRESULT CCell_Guide::Initialize_Prototype()
{
    XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());
    
    void* pTest = nullptr;
    FAILED_CHECK_RETURN(__super::Initialize(pTest), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

    return S_OK;
}

HRESULT CCell_Guide::Initialize(void* pArg)
{
    // 이니셜라이즈 할때 뭘 해야할까?
    // 포인트를 0,1,2 순서로 먼저 만들까
    // 일단 패스
    
    // 행렬 정보는 필요해 보임


    return S_OK;
}

void CCell_Guide::Priority_Update(_float fTimeDelta)
{

}

void CCell_Guide::Update(_float fTimeDelta, _vector vCoord)
{
    // 처음엔 0
    m_vecBufferComs.back()->Modify_VertexPoint(m_iIndex, vCoord);

    // 클릭하면 인덱스 증가
    // 한 프레임 사이에 갑자기 훅 증가하네
    if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
    {
        //m_bIsClicked = true;

        if (2 <= m_iIndex)
        {
            m_pVIBufferCom =
                dynamic_cast<CNavi_Cell*>(m_pGameInstance->
                    Clone_Prototype(PROTOTYPE::TYPE_COMPONENT,
                        LEVEL_TOOL, PRO_COM_VI_GUIDE));

            m_vecBufferComs.push_back(m_pVIBufferCom);

            m_iIndex = 0;

            return;
        }
        m_iIndex++;
        //한번만 실행하게 bool 타입 하나 추가 해야 할 듯 함
        //m_bIsClicked = false;
    }
}

void CCell_Guide::Late_Update(_float fTimeDelta)
{
    // 여기서 정점들의 위치를 선언하면 matrix 값을 직접 수정해야 함
    if (m_pGameInstance->Get_DIKeyState(DIK_MINUS) && m_iIndex > 0)
        m_iIndex--;

    if (m_pGameInstance->Get_DIKeyState(DIK_DELETE))
        m_vecBufferComs.pop_back();

    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CCell_Guide::Render()
{
    // 렌더를 어떻게 할까 제일 마지막에 있는 녀석만 렌더하면 될 듯 하지만
    // 내가 메시를 어디어디 찍었는지도 알아야 함
    FAILED_CHECK_RETURN(Bind_SR(), E_FAIL);

    m_pShaderCom->Begin(0);

    if(m_bIsModify)
    {
        for (auto& iter : m_vecBufferComs)
        {
            iter->Bind_Input_Assembler();
            iter->Render();
        }

    }
    //else


	return S_OK;
}

HRESULT CCell_Guide::Bind_SR()
{
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix_Ptr()), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, CPipeLine::D3DTS_VIEW), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, CPipeLine::D3DTS_PROJ), E_FAIL);
    //FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4)), E_FAIL);

	return S_OK;
}

HRESULT CCell_Guide::Ready_Component()
{
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_TOOL, PRO_COM_VI_GUIDE,
        reinterpret_cast<CComponent**>(&m_pVIBufferCom), TEXT("Com_VIBuffer")), E_FAIL);
    
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_TOOL, PRO_SHADER_CELL,
        reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

	return S_OK;
}

HRESULT CCell_Guide::Clone_VIBuffer()
{
    m_pVIBufferCom =
        dynamic_cast<CNavi_Cell*>(m_pGameInstance->
            Clone_Prototype(PROTOTYPE::TYPE_COMPONENT,
            LEVEL_TOOL, PRO_COM_VI_GUIDE));

    // 여기 수정해야 함
    // 처음 생성했을 때에만
    if(m_vecBufferComs.empty())
        m_vecBufferComs.push_back(m_pVIBufferCom);

    Safe_AddRef(m_pVIBufferCom);

    m_bIsModify = true;

    return S_OK;
}

void CCell_Guide::Check_Cell_Translation()
{
    // Buffer들의 위치 정점을 다 수정하였는지 체크
}

CCell_Guide* CCell_Guide::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCell_Guide* pInstance = new CCell_Guide(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Navi_Cell");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCell_Guide::Clone(void* pArg)
{
    CCell_Guide* pInstance = new CCell_Guide(*this);

    // GameObject라서 좀 불합리한데 귀찮다 그냥하자
    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Navi_Cell");
    }
        Safe_Release(pInstance);

    return pInstance;
} 

void CCell_Guide::Free()
{
    __super::Free();

    for (auto& iter : m_vecBufferComs)
        Safe_Release(m_pVIBufferCom);

    m_vecBufferComs.clear();

    Safe_Release(m_pShaderCom);

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);
}
