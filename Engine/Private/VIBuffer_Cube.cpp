#include "VIBuffer_Cube.h"

VIBuffer_Cube::VIBuffer_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: VIBuffer{ pDevice, pContext }
{
}

VIBuffer_Cube::VIBuffer_Cube(const VIBuffer_Cube& Prototype)
	: VIBuffer{ Prototype }
{
}

HRESULT VIBuffer_Cube::Initialize_Prototype()
{
	m_iVertexStride = sizeof(VTXCUBE);
	m_iNumVertices = 8;											// 정점 개수
	m_iIndexStride = 2;											// 인덱스 버퍼의 데이터 크기 2 또는 4byte로 설정함
	m_iNumIndices = 36;											// 인덱스 버퍼의 개수
	m_iNumVertexBuffers = 1;									// 
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;						// 16비트 정수로 세팅(인덱스 버퍼 전용)
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;			// 연속으로 삼각형을 그리겠다는 세팅

#pragma region VERTEXBUFFER
	// 구조체 초기화
	ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;   // 버퍼의 크기를 바이트 단위로 지정
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;					 // 버퍼의 사용 방법 지정
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			 // 무슨 버퍼로 사용할지 설정
	m_BufferDesc.StructureByteStride = m_iVertexStride;			 // 구조화된 버퍼에서 각 구조체의 크기를 바이트 단위로 지정하는 변수
	m_BufferDesc.CPUAccessFlags = 0;							 // CPU가 리소스를 접근할 권한을 설정
	m_BufferDesc.MiscFlags = 0;									 // 추가적인 버퍼 특성을 지정

	VTXCUBE* pVertices = new VTXCUBE[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXCUBE) * m_iNumVertices);

	// 큐브의 중심(원점)으로부터 Texcoord의 정점의 방향으로 벡터를 구해서 위치를 구한다
	// 
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = pVertices[0].vPosition; /*_float(0,0,0,0)*/;

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = pVertices[1].vPosition;

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = pVertices[2].vPosition;

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = pVertices[3].vPosition;

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexcoord = pVertices[4].vPosition;

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexcoord = pVertices[5].vPosition;

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexcoord = pVertices[6].vPosition;

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexcoord = pVertices[7].vPosition;

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

	pIndices[0] = 1; pIndices[1] = 5; pIndices[2] = 6;
	pIndices[3] = 1; pIndices[4] = 6; pIndices[5] = 2;

	pIndices[6] = 4; pIndices[7] = 0; pIndices[8] = 3;
	pIndices[9] = 4; pIndices[10] = 3; pIndices[11] = 7;


	pIndices[12] = 4; pIndices[13] = 5; pIndices[14] = 1;
	pIndices[15] = 4; pIndices[16] = 1; pIndices[17] = 0;


	pIndices[18] = 3; pIndices[19] = 2; pIndices[20] = 6;
	pIndices[21] = 3; pIndices[22] = 6; pIndices[23] = 7;


	pIndices[24] = 7; pIndices[25] = 6; pIndices[26] = 5;
	pIndices[27] = 7; pIndices[28] = 5; pIndices[29] = 4;


	pIndices[30] = 0; pIndices[31] = 1; pIndices[32] = 2;
	pIndices[33] = 0; pIndices[34] = 2; pIndices[35] = 3;


	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);


#pragma endregion

	return S_OK;
}

HRESULT VIBuffer_Cube::Initialize(void* pArg)
{
	return S_OK;
}

VIBuffer_Cube* VIBuffer_Cube::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	VIBuffer_Cube* pInstance = new VIBuffer_Cube(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : VIBuffer_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component* VIBuffer_Cube::Clone(void* pArg)
{
	Component* pInstance = new VIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : VIBuffer_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void VIBuffer_Cube::Free()
{
	__super::Free();
}
