#include "Mesh.h"

Mesh::Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : VIBuffer{ pDevice, pContext }
{
}

Mesh::Mesh(const Mesh& Prototype)
    : VIBuffer{ Prototype }
{
}

HRESULT Mesh::Initialize_Prototype(const aiMesh* pAIMesh)
{
	m_iVertexStride = sizeof(VTXMESH);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iIndexStride = 4;

	// mNumFaces = 면의 개수를 의미 -> 모든 면을 삼각형으로만 그려놨었다 그래서 삼각형의 개수를 넣어줘야 하는 것임
	// 따라서 면의 개수 * 3을 해야 인덱스의 개수가 된다
	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_iNumVertexBuffers = 1;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;

#pragma region VERTEXBUFFER

	ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;		
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXMESH*		pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; ++i)
	{
		// vPosition은 float3의 자료형을 사용하고 있음 
		// pAIMesh->mVertices 또한 float3 자료형을 사용 중이다
		// 따라서 효율적인 memcpy를 통해 메모리 복사를 한다
		
		// 여기에서 메쉬가 가지고 있는 정보들을 전달해주는 것이 좋다
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));

		// 0번째정점에 선언되어 있는 Texcoord를 설정하려고 하는 것이기에 [0][i]
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}
	
	ZeroMemory(&m_InitialData, sizeof(m_InitialData));
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

	// 위에 m_iIndexStride를 4로 설정하였기에 _uint
	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint	iNumIndices = { };

	// 몇 번째 면의 0, 1, 2 인덱스의 정보를 담아주는 것
	for (size_t i = 0; i < pAIMesh->mNumFaces; ++i)
	{
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[0];
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[1];
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);


#pragma endregion

    return S_OK;
}

HRESULT Mesh::Initialize(void* pArg)
{
    return S_OK;
}

Mesh* Mesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh)
{
	Mesh* pInstance = new Mesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pAIMesh)))
	{
		MSG_BOX("Failed To Created : Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component* Mesh::Clone(void* pArg)
{
	Component* pInstance = new Mesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Mesh::Free()
{
    __super::Free();
}
