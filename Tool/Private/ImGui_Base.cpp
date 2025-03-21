#include "ImGui_Base.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/ImGuizmo.h"

#include "GameInstance.h"
#include "Util.h"

CImGui_Base::CImGui_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

ImVec2 CImGui_Base::Set_Button_Offset(const char* _cButtonName, _float fX, _float fY)
{
	ImVec2 windowSize = ImGui::GetWindowSize();

	ImVec2 textSize = ImGui::CalcTextSize(_cButtonName);
	ImVec2 buttonPadding = ImGui::GetStyle().FramePadding;
	ImVec2 buttonSize = ImVec2(textSize.x + buttonPadding.x * 2, textSize.y + buttonPadding.y * 2);

	float posX = windowSize.x - buttonSize.x + fX;
	float posY = windowSize.y - buttonSize.y + fY;

	ImVec2 ButtonPos = { posX , posY };

	return ButtonPos;
}

void CImGui_Base::Get_PrototypeList(const wstring _strDest, const wstring _strSour)
{

	if(ImGui::TreeNode("Prototype_List"))
	{
		// 모든 리스트를 찾기
		// 이니셜라이즈 때 해버리면 읽지 못하는 순서여서 여기에 선언
		m_pGameInstance->Collect_ProtoTag(LEVEL_TOOL);

		if(m_vecProtoNames.empty())
		{
			m_vecProtoNames = *m_pGameInstance->Get_PrototypeTag(LEVEL_TOOL);
		}

		if (m_vecProtoTag.empty())
		{
			for (size_t i = 0; i < m_vecProtoNames.size(); i++)
			{
				// 필터로 가려진 문자열들 추려내고 저장
				if (m_vecProtoNames[i].find(_strDest) != std::string::npos ||
					m_vecProtoNames[i].find(_strSour) != std::string::npos)
				{
					m_vecProtoTag.push_back(m_vecProtoNames[i]);
				}
			}
		}

		// 루프 조금이라도 적게 돌리려고 이렇게 함
		for (size_t i = 0; i < m_vecProtoTag.size(); i++)
		{
			string strName1 = Util::WstrToStr(m_vecProtoTag[i]);
			const char* cstr = strName1.c_str();

			if (m_vecProtoTag[i].find(_strDest) != std::string::npos)
			{
				// 여기까지는 문제가 없음
				if (ImGui::Button(cstr))
				{
					// 모델 이름 저장하기
					m_strModelName = m_vecProtoTag[i];

					size_t iDestLast = m_vecProtoTag[i].rfind(TEXT("_"));
					wstring strDest = m_vecProtoTag[i].substr(iDestLast + 1);

					// 여기에서 문자열 찾기
					for (size_t j = 0; j < m_vecProtoTag.size(); j++)
					{
						if(m_vecProtoTag[j].find(_strSour) != std::string::npos)
						{
							size_t iSourLast = m_vecProtoTag[j].rfind(TEXT("_"));
							wstring strSour = m_vecProtoTag[j].substr(iSourLast + 1);

							if (strDest == strSour)
								m_strObjectName = m_vecProtoTag[j];

							int a = 10;
						}

					}
				}
			}
		}

		ImGui::TreePop();
	}

}


void CImGui_Base::Free()
{
	__super::Free();


	m_vecProtoTag.clear();
	m_vecProtoNames.clear();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
