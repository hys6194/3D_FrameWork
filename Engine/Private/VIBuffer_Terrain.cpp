#include "VIBuffer_Terrain.h"

VIBuffer_Terrain::VIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: VIBuffer { pDevice, pContext }
{

	
}

VIBuffer_Terrain::VIBuffer_Terrain(const VIBuffer_Terrain& Prototype)
	: VIBuffer{ Prototype }
{
}

HRESULT VIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{
	m_iVertexStride = sizeof(VTXNORTEX);

	_ulong		dwByte = {};

	HANDLE		hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER		fh{};
	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);

	BITMAPINFOHEADER		ih{};	
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	_uint* pPixel = new _uint[ih.biWidth * ih.biHeight];
	ReadFile(hFile, pPixel, sizeof(_uint) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iIndexStride = 4;
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;	// 버텍스 버퍼 * 2 = 사각형에 존재하는 삼각형 * 인덱스 버퍼
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEXBUFFER


	VTXNORTEX*		pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			// 2월 6일 복습 부분

			pVertices[iIndex].vPosition = _float3(j, pPixel[iIndex] & 0x000000ff, i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);

			// vTexcoord의 경우, i값과 k값이 1,1이 되어야 함
			// 왼쪽 하단을 0,0으로 잡고 오른쪽 상단을 1,1로 잡았기 때문
			// 루프가 0부터 시작하므로 m_iNumVertices - 1을 해야 루프 값만큼 나누기가 진행 된다
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}

	


#pragma endregion



	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

#pragma region INDEXBUFFER


	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pPixel);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iIndexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;


	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;


	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

#pragma endregion


	return S_OK;
}

HRESULT VIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

VIBuffer_Terrain* VIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath)
{
	VIBuffer_Terrain* pInstance = new VIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
	{
		MSG_BOX("Failed To Created : VIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component* VIBuffer_Terrain::Clone(void* pArg)
{
	Component* pInstance = new VIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : VIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void VIBuffer_Terrain::Free()
{
	__super::Free();


}
