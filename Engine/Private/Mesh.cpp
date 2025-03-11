#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CVIBuffer{ pDevice, pContext }
{
}

CMesh::CMesh(const CMesh& Prototype)
    : CVIBuffer{ Prototype }
{
}

HRESULT CMesh::Initialize_Prototype(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, MODELTYPE eType, const vector<class CBone*>& Bones)
{
	strcpy_s(m_szName, pAIMesh->mName.data);

	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iIndexStride = 4;

	// mNumFaces = 면의 개수를 의미 -> 모든 면을 삼각형으로만 그려놨었다 그래서 삼각형의 개수를 넣어줘야 하는 것임
	// 따라서 면의 개수 * 3을 해야 인덱스의 개수가 된다
	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;



#pragma region VERTEXBUFFER

	HRESULT hr = eType == MODELTYPE::TYPE_NONANIM ?
		Ready_VertexBuffer_NonAnim(pAIMesh, PreTransformMatrix) :
		Ready_VertexBuffer_Anim(pAIMesh, Bones);

	if (FAILED(hr))
		return E_FAIL;

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

HRESULT CMesh::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CMesh::Bind_BoneMatrix(CShader* pShader, const _char* pContantName, const vector<class CBone*>& Bones)
{
	ZeroMemory(m_matBone, sizeof(_float4x4) * 512);

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		XMStoreFloat4x4(&m_matBone[i],
			XMLoadFloat4x4(&m_OffsetMatrix[i]) *
			Bones[m_vecBone[i]]->Get_CombinedTransformationMatrix());
	}

	pShader->Bind_Matrices(pContantName, m_matBone, m_iNumBones);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{


	m_iVertexStride = sizeof(VTXMESH);
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; ++i)
	{
		// vPosition은 float3의 자료형을 사용하고 있음 
		// pAIMesh->mVertices 또한 float3 자료형을 사용 중이다
		// 따라서 효율적인 memcpy를 통해 메모리 복사를 한다

		// 여기에서 메쉬가 가지고 있는 정보들을 전달해주는 것이 좋다
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));

		// 동차 좌표로 만들어 준다 
		// w 값을 1로 만들어서 위치 좌표로 만들어 준다는 의미
		XMStoreFloat3(&pVertices[i].vPosition,
			XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

		// 0번째정점에 선언되어 있는 Texcoord를 설정하려고 하는 것이기에 [0][i]
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

// 뼈를 회전하고 정점을 붙이는 구조임

HRESULT CMesh::Ready_VertexBuffer_Anim(const aiMesh* pAIMesh, const vector<CBone*>& Bones)
{
	m_iVertexStride = sizeof(VTXANIMESH);
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXANIMESH* pVertices = new VTXANIMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2)); ;
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	/* 이 메시에 영향을 주는 뼈의 갯수 .*/
	m_iNumBones = pAIMesh->mNumBones;

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		/* i번째 뼈는 몇개의 정점에 영향을 주는가?! */
		aiBone* pAIBone = pAIMesh->mBones[i];

		_uint		iNumWeights = pAIBone->mNumWeights;

		/* 이 메시에게 영향을 주는 뼈의 이름과 같은 이름을 가진 뼈를
		모델이 들고 있는 뼈들에게서 찾자. */

		_uint		iBoneIndex = {};

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
			{
				if (true == pBone->Compare_Name(pAIBone->mName.data))
					return true;

				++iBoneIndex;

				return false;
			});

		m_vecBone.push_back(iBoneIndex);

		_float4x4		OffsetMatrix;

		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		XMStoreFloat4x4(&OffsetMatrix,
			XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_OffsetMatrix.push_back(OffsetMatrix);

		for (size_t j = 0; j < iNumWeights; j++)
		{
			/* i번째 뼈가 영향을 주는 j번째 정점의 정보를 알아보자. */
			/* i번째 뼈가 영향을 주는 j번째 정점의 인덱스 */
			// pAIMesh->mBones[i]->mWeights[j].mVertexId

			/* i번째 뼈가 영향을 주는 j번째 정점의 가중치(i번째 뼈는 j번째 정점에 얼마나 영향(0.f ~ 1.f)을 줄꺼야!!) */
			// pAIMesh->mBones[i]->mWeights[j].mWeight			

			if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x)
			{
				/* pAIMesh->mBones[i]->mWeights[j].mVertexId번째 정점에 영향을 주는 첫번째 뼈*/
				/*pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendIndex.x*/
				/* 이 메시에 영향을 주는 뼈들중 몇번째?(vBlendIndex)*/
				pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendIndex.x = i;
				pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
			}

			else if (0.f == pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendWeight.y)
			{
				/* pAIMesh->mBones[i]->mWeights[j].mVertexId번째 정점에 영향을 주는 두번째 뼈*/
				/*pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendIndex.y*/
				pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendIndex.y = i;
				pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendWeight.y = pAIMesh->mBones[i]->mWeights[j].mWeight;
			}

			else if (0.f == pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendWeight.z)
			{
				/* pAIMesh->mBones[i]->mWeights[j].mVertexId번째 정점에 영향을 주는 세번째 뼈*/
				/*pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendIndex.z*/
				pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendIndex.z = i;
				pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendWeight.z = pAIMesh->mBones[i]->mWeights[j].mWeight;
			}

			else
			{
				/* pAIMesh->mBones[i]->mWeights[j].mVertexId번째 정점에 영향을 주는 네번째 뼈*/
				/*pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendIndex.w*/
				pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendIndex.w = i;
				pVertices[pAIMesh->mBones[i]->mWeights[j].mVertexId].vBlendWeight.w = pAIMesh->mBones[i]->mWeights[j].mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		_uint		iBoneIndex = {};

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
			{
				if (true == pBone->Compare_Name(m_szName))
					return true;

				++iBoneIndex;

				return false;
			});

		_float4x4 OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

		m_OffsetMatrix.push_back(OffsetMatrix);

		m_vecBone.push_back(iBoneIndex);
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh, MODELTYPE eType, const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pAIMesh, PreTransformMatrix, eType, Bones)))
	{
		MSG_BOX("Failed To Created : Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	CComponent* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh::Free()
{
    __super::Free();
}



// 관익이 형 코드
//m_iNumBones = pAIMesh->mNumBones;
//
//// For Debug 
//// vector<int> vertexCounter;
//// vertexCounter.resize(m_iNumVertices);
//
//// 중복된 본이 들어가는 것을 막는다
//vector<set<const char*>> vertexBones;
//vertexBones.resize(m_iNumVertices);
//
//for (size_t i = 0; i < m_iNumBones; ++i)
//{
//	/* i번째 뼈는 몇개의 정점에 영향을 주는가?! */
//	_uint      iNumWeights = pAIMesh->mBones[i]->mNumWeights;
//
//	/* 이 메시에게 영향을 주는 뼈의 이름과 같은 이름을 가진 뼈를
//	모델이 들고 있는 뼈들에게서 찾자. */
//
//	_uint      iBoneIndex = {};
//
//	auto   iter = find_if(m_vecBone.begin(), Bones.end(), [&](CBone* pBone)->_bool
//		{
//			if (true == pBone->Compare_Name(pAIMesh->mBones[i]->mName.data))
//				return true;
//
//			++iBoneIndex;
//
//			return false;
//		});
//
//	m_Bones.push_back(iBoneIndex);
//
//	const char* boneName = pAIMesh->mBones[i]->mName.data;
//
//	_float4x4      OffsetMatrix;
//
//	memcpy(&OffsetMatrix, &pAIMesh->mBones[i]->mOffsetMatrix, sizeof(_float4x4));
//
//	XMStoreFloat4x4(&OffsetMatrix,
//		XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
//
//	m_OffsetMatrix.push_back(OffsetMatrix);
//
//	for (size_t j = 0; j < iNumWeights; ++j)
//	{
//		aiVertexWeight& vertexWeight = pAIMesh->mBones[i]->mWeights[j];
//
//		if (vertexBones[vertexWeight.mVertexId].find(boneName) != vertexBones[vertexWeight.mVertexId].end())
//			continue;
//
//		vertexBones[vertexWeight.mVertexId].insert(boneName);
//
//		// For Debug
//		// vertexCounter[vertexWeight.mVertexId]++;
//
//		if (0.f == pVertices[vertexWeight.mVertexId].vBlendWeight0.x)
//		{
//			
//		}
//
//
//	}
//}