#include "Tool_Manager.h"
#include "GameInstance.h"

#include "ImGui_UI.h"
#include "ImGui_Map.h"

Tool_Manager::Tool_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	//Safe_AddRef(m_pGameInstance);
}

HRESULT Tool_Manager::Initialize()
{

	m_pUI = CImGui_UI::Create(m_pDevice, m_pContext);
	NULL_CHECK_RETURN(m_pUI, E_FAIL);

	m_pMap = CImGui_Map::Create(m_pDevice, m_pContext);
	NULL_CHECK_RETURN(m_pMap, E_FAIL);


	return S_OK;
}

void Tool_Manager::Update(_float fTimeDelta)
{
	m_pUI->Update(fTimeDelta);
	m_pMap->Update(fTimeDelta);
}

void Tool_Manager::Late_Update(_float fTimeDelta)
{
}

HRESULT Tool_Manager::Render()
{
	return S_OK;
}

Tool_Manager* Tool_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Tool_Manager* pInstance = new Tool_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Tool_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Tool_Manager::Free()
{
	__super::Free();


	Safe_Release(m_pUI);
	Safe_Release(m_pMap);

	//Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

}
