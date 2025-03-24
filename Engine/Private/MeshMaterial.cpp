#include "MeshMaterial.h"
#include "Shader.h"

CMeshMaterial::CMeshMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CMeshMaterial::Initialize(const aiMaterial* pAIMaterial, const _char* pModelFilePath)
{
	for (size_t i = 1; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		// 메시가 가지고 있는 텍스쳐를 가져옴
		// 그 개수만큼 순회하여 텍스쳐의 정보를 읽어올 것임
		_uint	iNumSRVs = pAIMaterial->GetTextureCount(aiTextureType(i));

		for (size_t j = 0; j < iNumSRVs; j++)
		{
			ID3D11ShaderResourceView* pSRV = { nullptr };

			// 처음 추출하게 되면 추출했을때의 텍스쳐 경로를 받아오게 된다
			aiString	strPath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(i), j, &strPath)))
				continue;

			char		szDrive[MAX_PATH] = {};
			char		szDir[MAX_PATH] = {};
			char		szFileName[MAX_PATH] = {};
			char		szExt[MAX_PATH] = {};

			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			char		szFullPath[MAX_PATH] = {};

			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDir);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar		szTextureFilePath[MAX_PATH] = {};

			// MultiByteToWideChar : 멀티바이트 문자열(예: ANSI, UTF-8 등)을 
			// 와이드 문자 문자열(UTF-16)로 변환하는 Windows API 함수
			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath),
				szTextureFilePath, MAX_PATH);

			HRESULT			hr = {};

			if (0 == strcmp(szExt, ".dds"))
				hr = DirectX::CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
			else
				hr = DirectX::CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);

			if (FAILED(hr))
				return E_FAIL;

			m_vecMaterial[i].push_back(pSRV);
		}
	}


	return S_OK;
}

HRESULT CMeshMaterial::Bind_SR(CShader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iTextureIndex)
{
	if (m_vecMaterial[eMaterialType].empty() ||
		iTextureIndex >= m_vecMaterial[eMaterialType].size())
		return E_FAIL;


    return pShader->Bind_SRV(pConstantName, m_vecMaterial[eMaterialType][iTextureIndex]);
}

CMeshMaterial* CMeshMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMaterial* pAIMaterial, const _char* pModelFilePath)
{
	CMeshMaterial* pInstance = new CMeshMaterial(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pAIMaterial, pModelFilePath)))
	{
		MSG_BOX("Failed To Created : MeshMaterial");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CMeshMaterial::Initialize(const aiMaterial* _pAIMaterial, const _char* _pModelFilePath, ofstream& _OutStream)
{
	vector<_string> Materials[AI_TEXTURE_TYPE_MAX];


	for (size_t i = 1; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		_uint iNumSRVs = _pAIMaterial->GetTextureCount(aiTextureType(i));


		for (size_t j = 0; j < iNumSRVs; j++)
		{
			ID3D11ShaderResourceView* pSRV = { nullptr };

			aiString strPath = { };

			if (FAILED(_pAIMaterial->GetTexture(aiTextureType(i), j, &strPath)))
				return E_FAIL;

			_char szDrive[MAX_PATH] = { };
			_char szDir[MAX_PATH] = { };
			_char szFileName[MAX_PATH] = { };
			_char szExt[MAX_PATH] = { };

			_splitpath_s(_pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			_char szFullPath[MAX_PATH] = { };
			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDir);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar szTextureFilePath[MAX_PATH] = { };
			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szTextureFilePath, MAX_PATH);

			HRESULT hr = { };

			if (strcmp(szExt, ".dds") == 0)
				hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
			else
				hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);

			if (FAILED(hr))
				return E_FAIL;

			Materials[i].push_back(szFullPath);

			m_vecMaterial[i].push_back(pSRV);
		}
	}

	for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		_uint iSize = Materials[i].size();
		_OutStream.write(reinterpret_cast<const char*>(&iSize), sizeof(_uint));
		for (size_t j = 0; j < iSize; j++)
		{
			_uint iLength = Materials[i][j].size();
			_char szTexturePath[MAX_PATH] = { };
			strcpy_s(szTexturePath, Materials[i][j].c_str());
			auto Temp = Materials[i];
			_OutStream.write(reinterpret_cast<const char*>(&iLength), sizeof(_uint));
			_OutStream.write(reinterpret_cast<const char*>(szTexturePath), sizeof(_char) * iLength);
		}
	}

	return S_OK;
}

HRESULT CMeshMaterial::Initialize(ifstream& _InStream)
{
	for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		_uint iReadByte = { };
		_char szMaterialAttributePath[MAX_PATH] = { };
		_InStream.read(reinterpret_cast<char*>(&iReadByte), sizeof(_uint));


		for (size_t j = 0; j < iReadByte; j++)
		{
			ID3D11ShaderResourceView* pSRV = { nullptr };

			_uint iLength = { };
			_InStream.read(reinterpret_cast<char*>(&iLength), sizeof(_uint));
			_InStream.read(reinterpret_cast<char*>(szMaterialAttributePath), sizeof(_char) * iLength);

			_char szExt[MAX_PATH] = { };
			_splitpath_s(szMaterialAttributePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

			_tchar szTextureFilePath[MAX_PATH] = { };
			MultiByteToWideChar(CP_ACP, 0, szMaterialAttributePath, MAX_PATH, szTextureFilePath, MAX_PATH);

			HRESULT hr = { };

			if (strcmp(szExt, ".dds") == 0)
				hr = CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
			else
				hr = CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);

			if (FAILED(hr))
				return E_FAIL;

			m_vecMaterial[i].push_back(pSRV);
		}
	}

	return S_OK;
}

CMeshMaterial* CMeshMaterial::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const aiMaterial* _pAIMaterial, const _char* _pModelFilePath, ofstream& _OutStream)
{
	CMeshMaterial* pInstance = new CMeshMaterial(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pAIMaterial, _pModelFilePath, _OutStream)))
	{
		MSG_BOX("Failed To Created : CMeshMaterial");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMeshMaterial* CMeshMaterial::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, ifstream& _InStream)
{
	CMeshMaterial* pInstance = new CMeshMaterial(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_InStream)))
	{
		MSG_BOX("Failed To Created : CMeshMaterial");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshMaterial::Free()
{
	__super::Free();

	// AI_TEXTURE_TYPE_MAX : assimp에서 지원하는 텍스쳐 유형의 최대 개수를 의미함
	// 노션에서 그림으로 그려냈으니 그걸 참고하면 좋다
	for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		for (auto& pSRV : m_vecMaterial[i])
			Safe_Release(pSRV);

		m_vecMaterial[i].clear();
	}

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}


