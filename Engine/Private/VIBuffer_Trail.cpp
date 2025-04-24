#include "VIBuffer_Trail.h"

#include "Normal_Trail.h"
//#include "Sword_Trail.h"
//#include "Motion_Trail.h"

CVIBuffer_Trail::CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& Prototype)
	: CVIBuffer{ Prototype }
{
}

HRESULT CVIBuffer_Trail::Initialize_Prototype()
{
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iNumVertices = 4;											
	m_iIndexStride = 2;											
	m_iNumIndices = 6;											
	m_iNumVertexBuffers = 1;									
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;						
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;			

#pragma region VERTEXBUFFER
	// 구조체 초기화
	ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;  
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	m_BufferDesc.StructureByteStride = m_iVertexStride;									
	m_BufferDesc.MiscFlags = 0;									

	VTXPOS* pVertices = new VTXPOS[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOS) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);	
	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);


#pragma endregion

#pragma region INDEXBUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iIndexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	// 2byte로 설정해서 _ushort로 동적할당
	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;


	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);


#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Trail::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Trail::Sword_Trail(_float fTimeDelta, void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Trail::Motion_Trail(_float fTimeDelta, void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Trail::Trail(_float fTimeDelta, void* pArg)
{
	CNormal_Trail::NORMALTRAIL_DESC* pDesc = static_cast<CNormal_Trail::NORMALTRAIL_DESC*>(pArg);

	// 0,3 번은 해당 객체의 중심 기준으로 +- size.y/2 한거 만큼
	// 1,2 번은 버퍼가 생성될 때 마다 0,3번의 정점을 받아야 함

	D3D11_MAPPED_SUBRESOURCE SubResourceDesc{};

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResourceDesc);

	// 셰이더로 색상을 정할 것이니 정점이 어떻게 움직이는지에 대해서만 생각하자
	VTXPOS* pVertices = (VTXPOS*)SubResourceDesc.pData;

	m_fTotalTime += fTimeDelta;

	//total타임이 
	if (pDesc->fTime.x <= m_fTotalTime)
	{
		pVertices[1].vPosition.y = m_fPrePos1.y;
		pVertices[2].vPosition.y = m_fPrePos2.y;
	}

	pVertices[0].vPosition.y = pDesc->fPos.y - pDesc->fTime.y;
	//pVertices[1].vPosition.y = 
	//pVertices[2].vPosition.y =
	pVertices[3].vPosition.y = pDesc->fPos.y + pDesc->fTime.y;



	m_pContext->Unmap(m_pVB,0);

	return S_OK;
}

CVIBuffer_Trail* CVIBuffer_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : VIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Trail::Clone(void* pArg)
{
	CComponent* pInstance = new CVIBuffer_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : VIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	__super::Free();
}
