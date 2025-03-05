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
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEXBUFFER
	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(j, (pPixel[iIndex] & 0x000000ff) / 10.f, i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}
#pragma endregion

#pragma region INDEXBUFFER
	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = {};

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vSourDir, vDestDir, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);


			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal,
				XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);
		}
	}

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
	}
#pragma endregion


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

	return S_OK;
}

//HRESULT VIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
//{
//	m_iVertexStride = sizeof(VTXNORTEX);
//
//	_ulong		dwByte = {};
//
//	HANDLE		hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
//	if (0 == hFile)
//		return E_FAIL;
//
//	BITMAPFILEHEADER		fh{};
//	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);
//
//	BITMAPINFOHEADER		ih{};	
//	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);
//
//	_uint* pPixel = new _uint[ih.biWidth * ih.biHeight];
//	ReadFile(hFile, pPixel, sizeof(_uint) * ih.biWidth * ih.biHeight, &dwByte, nullptr);
//
//	CloseHandle(hFile);
//
//	m_iNumVerticesX = ih.biWidth;
//	m_iNumVerticesZ = ih.biHeight;
//
//	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
//	m_iIndexStride = 4;
//	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;
//	m_iNumVertexBuffers = 1;
//	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
//	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//
//#pragma region VERTEXBUFFER
//
//
//	VTXNORTEX*		pVertices = new VTXNORTEX[m_iNumVertices];
//	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);
//
//	for (size_t i = 0; i < m_iNumVerticesZ; i++)
//	{
//		for (size_t j = 0; j < m_iNumVerticesX; j++)
//		{
//			_uint		iIndex = i * m_iNumVerticesX + j;
//
//			// 2월 6일 복습 부분
//
//			// 맵 높이 설정
//			pVertices[iIndex].vPosition = _float3(j, (pPixel[iIndex] & 0x000000ff) / 10.f, i);
//			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
//
//			// vTexcoord의 경우, i값과 k값이 1,1이 되어야 함
//			// 왼쪽 하단을 0,0으로 잡고 오른쪽 상단을 1,1로 잡았기 때문
//			// 루프가 0부터 시작하므로 m_iNumVertices - 1을 해야 루프 값만큼 나누기가 진행 된다
//			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
//		}
//	}
//
//#pragma endregion
//
//#pragma region INDEXBUFFER
//	_uint* pIndices = new _uint[m_iNumIndices];
//	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);
//
//	_uint		iNumIndices = {};
//
//	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
//	{
//		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
//		{
//			_uint		iIndex = i * m_iNumVerticesX + j;
//
//			_uint		iIndices[4] = {
//				iIndex + m_iNumVerticesX,
//				iIndex + m_iNumVerticesX + 1,
//				iIndex + 1,
//				iIndex
//			};
//
//			_vector		vSourDir, vDestDir, vNormal;
//
//			pIndices[iNumIndices++] = iIndices[0];
//			pIndices[iNumIndices++] = iIndices[1];
//			pIndices[iNumIndices++] = iIndices[2];
//
//
//			// 인덱스에 해당하는 노말벡터를 구해서 빛 반사에 적용할 것
//
//			// 0 -> 1, 1->2 방향의 벡터를 구하고 이에 외적하는 벡터로 노말 벡터를 구한다
//			// 이를 각각 인덱스들에 더해준다 
//
//			vSourDir = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
//			vDestDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
//			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));
//
//			XMStoreFloat3(&pVertices[iIndices[0]].vNormal,
//				XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
//			XMStoreFloat3(&pVertices[iIndices[1]].vNormal,
//				XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
//			XMStoreFloat3(&pVertices[iIndices[2]].vNormal,
//				XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
//
//
//			pIndices[iNumIndices++] = iIndices[0];
//			pIndices[iNumIndices++] = iIndices[2];
//			pIndices[iNumIndices++] = iIndices[3];
//
//			vSourDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
//			vDestDir = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
//			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));
//
//			XMStoreFloat3(&pVertices[iIndices[0]].vNormal,
//				XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
//			XMStoreFloat3(&pVertices[iIndices[2]].vNormal,
//				XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
//			XMStoreFloat3(&pVertices[iIndices[3]].vNormal,
//				XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);
//		}
//	}
//
//	for (size_t i = 0; i < m_iNumVertices; i++)
//	{
//		XMStoreFloat3(&pVertices[i].vNormal,
//			XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
//	}
//#pragma endregion
//
//
//	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
//	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	m_BufferDesc.StructureByteStride = m_iVertexStride;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//
//	ZeroMemory(&m_InitialData, sizeof m_InitialData);
//	m_InitialData.pSysMem = pVertices;
//
//	if (FAILED(__super::Create_Buffer(&m_pVB)))
//		return E_FAIL;
//
//	Safe_Delete_Array(pPixel);
//
//	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
//	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	m_BufferDesc.StructureByteStride = m_iIndexStride;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//
//	ZeroMemory(&m_InitialData, sizeof m_InitialData);
//	m_InitialData.pSysMem = pIndices;
//
//	if (FAILED(__super::Create_Buffer(&m_pIB)))
//		return E_FAIL;
//
//	Safe_Delete_Array(pVertices);
//	Safe_Delete_Array(pIndices);
//
//	return S_OK;
//}


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
