#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"

#include "GameInstance.h"
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

	_wstring wtest;
	const char* strMeshName = pAIMesh->mName.data;
	_wstring strFileType = TEXT("_Mesh.dat");

	size_t iTest = strlen(strMeshName);

	_wstring strFileName(strMeshName, strMeshName + strlen(strMeshName));

	wtest = Engine::strPathName + strFileName + strFileType;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);


	_ulong			dwByte = {};
	HANDLE			hFile = CreateFile(wtest.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	WriteFile(hFile, &m_iNumVertices, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &strMeshName, sizeof(iTest), &dwByte, nullptr);

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
		WriteFile(hFile, &pVertices[i].vPosition, sizeof(_float3), &dwByte, nullptr);

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));
		WriteFile(hFile, &pVertices[i].vNormal, sizeof(_float3), &dwByte, nullptr);


		// 0번째정점에 선언되어 있는 Texcoord를 설정하려고 하는 것이기에 [0][i]
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		WriteFile(hFile, &pVertices[i].vTexcoord, sizeof(_float2), &dwByte, nullptr);

		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		WriteFile(hFile, &pVertices[i].vTangent, sizeof(_float3), &dwByte, nullptr);

	}
	CloseHandle(hFile);


	//HANDLE hFile1 = CreateFile(wtest.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//ReadFile(hFile, &strMeshName, sizeof(_float3) * 3, &dwByte, nullptr);
	//ReadFile(hFile, &pVertices[0].vPosition, sizeof(_float3), &dwByte, nullptr);
	//ReadFile(hFile, &pVertices[0].vNormal, sizeof(_float3), &dwByte, nullptr);
	//ReadFile(hFile, &pVertices[0].vTexcoord, sizeof(_float2), &dwByte, nullptr);
	//ReadFile(hFile, &pVertices[0].vTangent, sizeof(_float3), &dwByte, nullptr);
	//
	//TCHAR debugMessage[256];
	//_stprintf_s(debugMessage, _T("vPosition: x = %.6f, y = %.6f, z = %.6f\n"),
	//	pVertices[0].vPosition.x, pVertices[0].vPosition.y, pVertices[0].vPosition.z);
	//OutputDebugString(debugMessage);
	//
	//TCHAR debugMessage1[256];
	//_stprintf_s(debugMessage1, _T("vNormal: x = %.6f, y = %.6f, z = %.6f\n"),
	//	pVertices[0].vNormal.x, pVertices[0].vNormal.y, pVertices[0].vNormal.z);
	//OutputDebugString(debugMessage1);
	//
	//TCHAR debugMessage2[256];
	//_stprintf_s(debugMessage2, _T("vTexcoord: x = %.6f, y = %.6ff\n"),
	//	pVertices[0].vTexcoord.x, pVertices[0].vTexcoord.y);
	//OutputDebugString(debugMessage2);
	//
	//TCHAR debugMessage3[256];
	//_stprintf_s(debugMessage3, _T("vTangent: x = %.6f, y = %.6f, z = %.6f\n"),
	//	pVertices[0].vTangent.x, pVertices[0].vTangent.y, pVertices[0].vTangent.z);
	//OutputDebugString(debugMessage3);


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

HRESULT CMesh::Initialize_Prototype(const aiMesh* _pAIMesh, MODELTYPE _eModelType, const vector<class CBone*>& _Bones, _fmatrix _PreTransformMatrix, ofstream& _OutStream)
{
	strcpy_s(m_szName, _pAIMesh->mName.data);
	m_iMaterialIndex = _pAIMesh->mMaterialIndex;
	m_iVertexStride = sizeof(VTXMESH);
	m_iNumVertices = _pAIMesh->mNumVertices;
	m_iIndexStride = 4;
	m_iNumIndices = _pAIMesh->mNumFaces * 3;

	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	_uint iWriteByte = sizeof(m_szName);
	_OutStream.write(reinterpret_cast<const char*>(&iWriteByte), sizeof(_uint));
	_OutStream.write(m_szName, sizeof(_char) * iWriteByte);

	_OutStream.write(reinterpret_cast<const char*>(&m_iMaterialIndex), sizeof(_uint));

	_OutStream.write(reinterpret_cast<const char*>(&m_iNumVertices), sizeof(_uint));
	_OutStream.write(reinterpret_cast<const char*>(&m_iNumIndices), sizeof(_uint));

#pragma region VERTEXBUFFER

	HRESULT hr = _eModelType == MODELTYPE::TYPE_NONANIM ?
		Ready_VertexBuffer_ForNonAnim_Save(_pAIMesh, _PreTransformMatrix, _OutStream) :
		Ready_VertexBuffer_ForAnim_Save(_pAIMesh, _Bones, _OutStream);

	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

#pragma region INDEXBUFFER
	ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));

	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices; // 정점의 크기 * 정점의 갯수
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;	// 버퍼 속성 : 정적 or 동적
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iIndexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint iNumIndices = { };

	for (size_t i = 0; i < _pAIMesh->mNumFaces; i++)
	{
		pIndices[iNumIndices++] = _pAIMesh->mFaces[i].mIndices[0];
		pIndices[iNumIndices++] = _pAIMesh->mFaces[i].mIndices[1];
		pIndices[iNumIndices++] = _pAIMesh->mFaces[i].mIndices[2];
	}

	_OutStream.write(reinterpret_cast<const char*>(pIndices), (sizeof(_uint) * m_iNumIndices));

	ZeroMemory(&m_InitialData, sizeof(m_InitialData));
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize_Prototype(MODELTYPE _eModelType, const vector<class CBone*>& _Bones, _fmatrix _PreTransformMatrix, ifstream& _InStream)
{
	_uint iReadByte = { };
	_char szBoneName[MAX_PATH] = { };
	_InStream.read(reinterpret_cast<char*>(&iReadByte), sizeof(_uint));
	_InStream.read(reinterpret_cast<char*>(szBoneName), sizeof(_char) * iReadByte);
	strcpy_s(m_szName, szBoneName);

	_InStream.read(reinterpret_cast<char*>(&m_iMaterialIndex), sizeof(_uint));
	_InStream.read(reinterpret_cast<char*>(&m_iNumVertices), sizeof(_uint));
	_InStream.read(reinterpret_cast<char*>(&m_iNumIndices), sizeof(_uint));

	m_iIndexStride = 4;

	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


#pragma region VERTEXBUFFER

	HRESULT hr = _eModelType == MODELTYPE::TYPE_NONANIM ?
		Ready_VertexBuffer_ForNonAnim_Load(_PreTransformMatrix, _InStream) :
		Ready_VertexBuffer_ForAnim_Load(_Bones, _InStream);

	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

#pragma region INDEXBUFFER
	ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));

	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices; // 정점의 크기 * 정점의 갯수
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;	// 버퍼 속성 : 정적 or 동적
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iIndexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_InStream.read(reinterpret_cast<char*>(pIndices), (sizeof(_uint) * m_iNumIndices));

	ZeroMemory(&m_InitialData, sizeof(m_InitialData));
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForNonAnim_Save(const aiMesh* _pAIMesh, _fmatrix PreTransformMatrix, ofstream& _OutStream)
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

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &_pAIMesh->mVertices[i], sizeof(_float3));
		_OutStream.write(reinterpret_cast<const char*>(&pVertices[i].vPosition), sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition,
			XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		memcpy(&pVertices[i].vNormal, &_pAIMesh->mNormals[i], sizeof(_float3));
		_OutStream.write(reinterpret_cast<const char*>(&pVertices[i].vNormal), sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

		memcpy(&pVertices[i].vTexcoord, &_pAIMesh->mTextureCoords[0][i], sizeof(_float2)); ;
		_OutStream.write(reinterpret_cast<const char*>(&pVertices[i].vTexcoord), sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &_pAIMesh->mTangents[i], sizeof(_float3));
		_OutStream.write(reinterpret_cast<const char*>(&pVertices[i].vTangent), sizeof(_float3));
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;


	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForAnim_Save(const aiMesh* _pAIMesh, const vector<class CBone*>& _Bones, ofstream& _OutStream)
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
		memcpy(&pVertices[i].vPosition, &_pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &_pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &_pAIMesh->mTextureCoords[0][i], sizeof(_float2)); ;
		memcpy(&pVertices[i].vTangent, &_pAIMesh->mTangents[i], sizeof(_float3));
	}

	/* 이 메시에 영향을 주는 뼈의 갯수 .*/
	m_iNumBones = _pAIMesh->mNumBones;

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		/* i번째 뼈는 몇개의 정점에 영향을 주는가?! */
		_uint		iNumWeights = _pAIMesh->mBones[i]->mNumWeights;

		_uint iBoneIndex = {};

		auto iter = find_if(_Bones.begin(), _Bones.end(), [&](CBone* _pBone)->_bool {
			if (_pBone->Compare_Name(_pAIMesh->mBones[i]->mName.data) == true)
				return true;

			++iBoneIndex;

			return false;
			});

		m_vecBone.push_back(iBoneIndex);

		_float4x4 OffsetMatrix = {};
		memcpy(&OffsetMatrix, &_pAIMesh->mBones[i]->mOffsetMatrix, sizeof(_float4x4));

		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_OffsetMatrix.push_back(OffsetMatrix);

		_uint iCnt = { };

		for (size_t j = 0; j < iNumWeights; j++)
		{
			aiVertexWeight& vertexWeight = _pAIMesh->mBones[i]->mWeights[j];

			if (0.f == pVertices[vertexWeight.mVertexId].vBlendWeight0.x)
			{
				pVertices[vertexWeight.mVertexId].vBlendIndex0.x = i;
				pVertices[vertexWeight.mVertexId].vBlendWeight0.x = vertexWeight.mWeight;
			}
			else if (0.f == pVertices[vertexWeight.mVertexId].vBlendWeight0.y)
			{
				pVertices[vertexWeight.mVertexId].vBlendIndex0.y = i;
				pVertices[vertexWeight.mVertexId].vBlendWeight0.y = vertexWeight.mWeight;
			}
			else if (0.f == pVertices[vertexWeight.mVertexId].vBlendWeight0.z)
			{
				pVertices[vertexWeight.mVertexId].vBlendIndex0.z = i;
				pVertices[vertexWeight.mVertexId].vBlendWeight0.z = vertexWeight.mWeight;
			}
			else if (0.f == pVertices[vertexWeight.mVertexId].vBlendWeight0.w)
			{
				pVertices[vertexWeight.mVertexId].vBlendIndex0.w = i;
				pVertices[vertexWeight.mVertexId].vBlendWeight0.w = vertexWeight.mWeight;
			}
			else if (0.f == pVertices[vertexWeight.mVertexId].vBlendWeight1.x)
			{
				pVertices[vertexWeight.mVertexId].vBlendIndex1.x = i;
				pVertices[vertexWeight.mVertexId].vBlendWeight1.x = vertexWeight.mWeight;
			}
			else if (0.f == pVertices[vertexWeight.mVertexId].vBlendWeight1.y)
			{
				pVertices[vertexWeight.mVertexId].vBlendIndex1.y = i;
				pVertices[vertexWeight.mVertexId].vBlendWeight1.y = vertexWeight.mWeight;
			}
			else if (0.f == pVertices[vertexWeight.mVertexId].vBlendWeight1.z)
			{
				pVertices[vertexWeight.mVertexId].vBlendIndex1.z = i;
				pVertices[vertexWeight.mVertexId].vBlendWeight1.z = vertexWeight.mWeight;
			}
			else if (0.f == pVertices[vertexWeight.mVertexId].vBlendWeight1.w)
			{
				pVertices[vertexWeight.mVertexId].vBlendIndex1.w = i;
				pVertices[vertexWeight.mVertexId].vBlendWeight1.w = vertexWeight.mWeight;
			}
		}
	}

	if (m_iNumBones == 0)
	{
		m_iNumBones = 1;

		_uint iBoneIndex = {};

		auto iter = find_if(_Bones.begin(), _Bones.end(), [&](CBone* _pBone)->_bool {
			if (_pBone->Compare_Name(m_szName) == true)
				return true;

			++iBoneIndex;

			return false;
			});

		m_vecBone.push_back(iBoneIndex);

		_float4x4 OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

		m_OffsetMatrix.push_back(OffsetMatrix);
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	_OutStream.write(reinterpret_cast<const char*>(&m_iNumBones), sizeof(_uint));
	_OutStream.write(reinterpret_cast<const char*>(pVertices), sizeof(VTXANIMESH) * m_iNumVertices);

	for (auto& OffsetMatrix : m_OffsetMatrix)
		_OutStream.write(reinterpret_cast<const char*>(&OffsetMatrix), sizeof(_float4x4));

	for (auto& Bone : m_vecBone)
		_OutStream.write(reinterpret_cast<const char*>(&Bone), sizeof(_int));


	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForNonAnim_Load(_fmatrix PreTransformMatrix, ifstream& _InStream)
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

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		_InStream.read(reinterpret_cast<char*>(&pVertices[i].vPosition), sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition,
			XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));
		_InStream.read(reinterpret_cast<char*>(&pVertices[i].vNormal), sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));
		_InStream.read(reinterpret_cast<char*>(&pVertices[i].vTexcoord), sizeof(_float2));
		_InStream.read(reinterpret_cast<char*>(&pVertices[i].vTangent), sizeof(_float3));

	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_ForAnim_Load(const vector<class CBone*>& _Bones, ifstream& _InStream)
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
	_InStream.read(reinterpret_cast<char*>(&m_iNumBones), sizeof(_uint));
	_InStream.read(reinterpret_cast<char*>(pVertices), sizeof(VTXANIMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		_float4x4 OffsetMatrix = { };
		_InStream.read(reinterpret_cast<char*>(&OffsetMatrix), sizeof(_float4x4));
		m_OffsetMatrix.push_back(OffsetMatrix);
	}

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		_int iBoneIndex = { };
		_InStream.read(reinterpret_cast<char*>(&iBoneIndex), sizeof(_int));
		m_vecBone.push_back(iBoneIndex);
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const aiMesh* _pAIMesh, MODELTYPE _eModelType, const vector<class CBone*>& _Bones, _fmatrix _PreTransformMatrix, ofstream& _OutStream)
{
	CMesh* pInstance = new CMesh(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(_pAIMesh, _eModelType, _Bones, _PreTransformMatrix, _OutStream)))
	{
		MSG_BOX("Failed To Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMesh* CMesh::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, MODELTYPE _eModelType, const vector<class CBone*>& _Bones, _fmatrix _PreTransformMatrix, ifstream& _InStream)
{
	CMesh* pInstance = new CMesh(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(_eModelType, _Bones, _PreTransformMatrix, _InStream)))
	{
		MSG_BOX("Failed To Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
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
//	m_vecBone.push_back(iBoneIndex);
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