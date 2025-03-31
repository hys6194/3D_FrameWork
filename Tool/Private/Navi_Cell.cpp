#include "Navi_Cell.h"

#include "GameInstance.h"

CNavi_Cell::CNavi_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CVIBuffer{ pDevice, pContext }
{
}

CNavi_Cell::CNavi_Cell(const CNavi_Cell& Prototype)
    : CVIBuffer { Prototype }
{
}

HRESULT CNavi_Cell::Initialize_Prototype()
{   
    return S_OK;
}

HRESULT CNavi_Cell::Initialize(void* pArg)
{
    //INSTVTX
    m_iVertexStride = sizeof(VTXPOS);
    m_iNumVertices = 3;
    m_iIndexStride = 2;
    m_iNumIndices = 4;
    m_iNumVertexBuffers = 1;
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;
    m_eTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    //D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    //D3D_PRIMITIVE_TOPOLOGY_LINESTRIP

#pragma region VTX

    ZeroMemory(&m_BufferDesc, sizeof(VTXPOS));
    m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = m_iVertexStride; // 이거 주의해서 생각하자

    VTXPOS* pVertices = new VTXPOS[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXPOS) * m_iNumVertices);

    pVertices[0].vPosition = _float3(0.f, 2.f, 0.f);
    pVertices[1].vPosition = _float3(0.f, 2.f, 1.f);
    pVertices[2].vPosition = _float3(1.f, 2.f, 0.f);

    ZeroMemory(&m_InitialData, sizeof m_InitialData);
    m_InitialData.pSysMem = pVertices;

    FAILED_CHECK_RETURN(__super::Create_Buffer(&m_pVB), E_FAIL);

    Safe_Delete_Array(pVertices);

#pragma endregion VTX

#pragma region IDX

    ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
    m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
    m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_BufferDesc.StructureByteStride = m_iIndexStride;
    m_BufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    m_BufferDesc.MiscFlags = 0;

    _ushort* pIndices = new _ushort[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

    pIndices[0] = 0;
    pIndices[1] = 1;
    pIndices[2] = 2;
    pIndices[3] = 0;

    ZeroMemory(&m_InitialData, sizeof m_InitialData);
    m_InitialData.pSysMem = pIndices;

    if (FAILED(__super::Create_Buffer(&m_pIB)))
        return E_FAIL;

    Safe_Delete_Array(pIndices);

#pragma endregion IDX


    return S_OK;
}

HRESULT CNavi_Cell::Render()
{
    if (nullptr == m_pContext)
        return E_FAIL;

    m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

    return S_OK;
}

void CNavi_Cell::Modify_VertexPoint(_uint iVertexIndex, _vector vCoord)
{
    // 여기에서 동적으로 버퍼의 위치를 설정해보자
    D3D11_MAPPED_SUBRESOURCE GuideSubResource{};

    m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &GuideSubResource);
    
    // 정점을 어떻게 수정할까
    VTXPOS* pVertices = (VTXPOS*)GuideSubResource.pData;

    _float4 fTest{};
    
    XMStoreFloat4(&fTest, vCoord);

    if (0 == iVertexIndex)
    {
        pVertices[iVertexIndex].vPosition = _float3(fTest.x, fTest.y + 0.3f, fTest.z);
        pVertices[iVertexIndex + 1].vPosition = _float3(fTest.x, fTest.y + 0.3f, fTest.z + 1);
        pVertices[iVertexIndex + 2].vPosition = _float3(fTest.x + 1, fTest.y + 0.3f, fTest.z );

        m_pVertices0.vPosition = pVertices[iVertexIndex].vPosition;

        m_eTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    }

    else
    {
        for (size_t i = iVertexIndex; i < 3; i++)
        {
            pVertices[i].vPosition = _float3(fTest.x, fTest.y + 0.3f, fTest.z);

            if (i == 1)
                m_pVertices1.vPosition = pVertices[i].vPosition;
        }

        if (iVertexIndex == 2)
        {

            m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

            _float3 fTest = pVertices[iVertexIndex].vPosition;

            _vector v1 = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&m_pVertices1.vPosition), XMLoadFloat3(&m_pVertices0.vPosition)));
            _vector v2 = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&fTest), XMLoadFloat3(&m_pVertices1.vPosition)));

            _vector vNor = { 0.f,0.f,0.f,0.f };
            vNor = XMVector3Cross(v1, v2);

            if (vNor.m128_f32[1] < 0)
            {
                pVertices[2].vPosition = m_pVertices1.vPosition;
                pVertices[1].vPosition = _float3(fTest.x, fTest.y, fTest.z);
            }

        }

        else
            m_eTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    }






    m_pContext->Unmap(m_pVB, 0);
}

HRESULT CNavi_Cell::Bind_Input_Assembler()
{
    ID3D11Buffer* pBuffer[] =
    {
        m_pVB,

    };

    _uint           iStrides[] = {
        m_iVertexStride,

    };

    _uint Offsets[] =
    {
        0,
    };

    m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pBuffer, iStrides, Offsets);
    m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
    m_pContext->IASetPrimitiveTopology(m_eTopology);

    return S_OK;
}

CNavi_Cell* CNavi_Cell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNavi_Cell* pInstance = new CNavi_Cell(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Navi_Cell");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CNavi_Cell::Clone(void* pArg)
{
    CComponent* pInstance = new CNavi_Cell(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Navi_Cell");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNavi_Cell::Free()
{
    __super::Free();
}
