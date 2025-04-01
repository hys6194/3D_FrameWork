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
    , m_bIsModify { Prototype.m_bIsModify }
{
}

HRESULT CCell_Guide::Initialize_Prototype()
{
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

    return S_OK;
}

void CCell_Guide::Priority_Update(_float fTimeDelta)
{

}

void CCell_Guide::Update(_float fTimeDelta, _vector vCoord)
{
    // 마우스 포인트 보정
    Correct_CellPoint(vCoord);

    // 가이드 Cell 조정
    if(nullptr != m_pVIBufferCom)
        m_pVIBufferCom->Modify_VertexPoint(m_iIndex, m_vPoint[m_iIndex]);

    // 클릭하면 인덱스 증가
    // 좌표 값을 저장해야 함
    if (m_pGameInstance->Mouse_Down(DIM_LB))
    {
        //m_bIsClicked = true;

        _float4 fTest;
        XMStoreFloat4(&fTest, vCoord);

        TCHAR debugMessage[256];
        _stprintf_s(debugMessage, _T("Debug_Value: x = %.6f, y = %.6f, z = %.6f\n"),
            fTest.x, fTest.y, fTest.z);
        OutputDebugString(debugMessage);



        if (2 <= m_iIndex)
        {
            Calculate_CellNorvec();

            m_vecBufferComs.push_back(m_pVIBufferCom);

            m_pVIBufferCom = dynamic_cast<CNavi_Cell*>(m_pGameInstance->
                    Clone_Prototype(PROTOTYPE::TYPE_COMPONENT,
                        LEVEL_TOOL, PRO_COM_VI_GUIDE));

            m_iIndex = 0;

            return;
        }

        m_iIndex++;

    }
}

void CCell_Guide::Late_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Key_Down(DIK_MINUS) && m_iIndex > 0)
        m_iIndex--;

    if (m_pGameInstance->Key_Down(DIK_DELETE) && !m_vecBufferComs.empty())
    {
        m_vecBufferComs.pop_back();
        m_vecCellPos.pop_back();
    }

    if (m_pGameInstance->Key_Down(DIK_F6))
    {
        m_vecBufferComs.clear();
        m_vecCellPos.clear();
    }


    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CCell_Guide::Render()
{
    // 렌더를 어떻게 할까 제일 마지막에 있는 녀석만 렌더하면 될 듯 하지만
    // 내가 메시를 어디어디 찍었는지도 알아야 함
    FAILED_CHECK_RETURN(Bind_SR(), E_FAIL);

    m_pShaderCom->Begin(1);

    if(!m_vecBufferComs.empty())
    {
        for (auto& iter : m_vecBufferComs)
        {
            iter->Bind_Input_Assembler();
            iter->Render();
        }

    }

    if(nullptr != m_pVIBufferCom)
    {
        m_pVIBufferCom->Bind_Input_Assembler();
        m_pVIBufferCom->Render();
    }

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
    //if(m_vecBufferComs.empty())
    //    m_vecBufferComs.push_back(m_pVIBufferCom);

    m_bIsModify = true;

    return S_OK;
}

HRESULT CCell_Guide::Delete_VIBuffer()
{

    return S_OK;
}

void CCell_Guide::Correct_CellPoint(_vector vCoord)
{
    m_vPoint[m_iIndex] = vCoord;

    if (m_vecCellPos.empty())
        m_vPoint[m_iIndex];

    _vector vDistance = { 0.5f, 0.5f, 0.5f, 1.f };

    for (auto iter = m_vecCellPos.rbegin();
        iter != m_vecCellPos.rend();
        iter++)
    {
        _vector vResult  = XMVectorNearEqual(iter->v0, m_vPoint[m_iIndex], vDistance);
        _vector vResult1 = XMVectorNearEqual(iter->v1, m_vPoint[m_iIndex], vDistance);
        _vector vResult2 = XMVectorNearEqual(iter->v2, m_vPoint[m_iIndex], vDistance);

        if (XMVector4EqualInt(vResult, XMVectorTrueInt()))
        {
            m_vPoint[m_iIndex] = iter->v0;
            break;
        }
        if (XMVector4EqualInt(vResult1, XMVectorTrueInt()))
        {
            m_vPoint[m_iIndex] = iter->v1;
            break;
        }
        if (XMVector4EqualInt(vResult2, XMVectorTrueInt()))
        {
            m_vPoint[m_iIndex] = iter->v2;
            break;
        }
    }    
}

void CCell_Guide::Calculate_CellNorvec()
{
    CELL_POS pDesc{};

    pDesc.v0 = m_vPoint[0];
    pDesc.v1 = m_vPoint[1];
    pDesc.v2 = m_vPoint[2];

    _vector v1 = XMVector4Normalize(XMVectorSubtract(m_vPoint[1], m_vPoint[0]));
    _vector v2 = XMVector4Normalize(XMVectorSubtract(m_vPoint[2], m_vPoint[1]));
    
    _vector vNor = {0.f,0.f,0.f,0.f};
    vNor = XMVector3Cross(v1, v2);
    
    if (vNor.m128_f32[1] < 0)
        swap(pDesc.v1, pDesc.v2);

    m_vecCellPos.push_back(pDesc);
}

void CCell_Guide::Save_Data()
{
    _ulong			dwByte = {};
    HANDLE			hFile = CreateFile(TEXT("../../Client/Bin/DataFiles/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    if (0 == hFile)
        return;
    //벡터에 담긴 데이터들을 기반으로 Navigation.dat 생성하자

    for (auto& iter : m_vecCellPos)
    {
        _float3 vPoints[3] = {};

        XMStoreFloat3(&vPoints[0], iter.v0);
        XMStoreFloat3(&vPoints[1], iter.v1);
        XMStoreFloat3(&vPoints[2], iter.v2);

        WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

    }



    CloseHandle(hFile);
}

void CCell_Guide::Load_Data()
{

    _ulong          dwByte = {};
    HANDLE          hFile = CreateFile(TEXT("../../Client/Bin/DataFiles/Navigation.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (0 == hFile)
        return ;

    //_uint iVecSize;
    //ReadFile(hFile, &iVecSize, sizeof(_uint), &dwByte, nullptr);

    _float3     vPoints[3] = {};

    while(true)
    { 
        ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
        if (0 == dwByte)
            break;

        CELL_POS Desc{};

        Desc.v0 = XMVectorSet(vPoints[0].x, vPoints[0].y, vPoints[0].z, 1.f);
        Desc.v1 = XMVectorSet(vPoints[1].x, vPoints[1].y, vPoints[1].z, 1.f);
        Desc.v2 = XMVectorSet(vPoints[2].x, vPoints[2].y, vPoints[2].z, 1.f);

        m_vecCellPos.push_back(Desc);

        m_pVIBufferCom =
            dynamic_cast<CNavi_Cell*>(m_pGameInstance->
                Clone_Prototype(PROTOTYPE::TYPE_COMPONENT,
                    LEVEL_TOOL, PRO_COM_VI_GUIDE));


        m_pVIBufferCom->Modify_VertexPoint(0,  Desc.v0);
        m_pVIBufferCom->Modify_VertexPoint(1,  Desc.v1);
        m_pVIBufferCom->Modify_VertexPoint(2,  Desc.v2);

        m_vecBufferComs.push_back(m_pVIBufferCom);

    }

    CloseHandle(hFile);

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
        Safe_Release(iter);
    
    m_vecBufferComs.clear();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);

    //Safe_Release(m_pDevice);
    //Safe_Release(m_pContext);
    //Safe_Release(m_pGameInstance);
}
