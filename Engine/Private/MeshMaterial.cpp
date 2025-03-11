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
