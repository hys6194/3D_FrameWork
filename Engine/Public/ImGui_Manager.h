#pragma once
#include "Base.h"

BEGIN(Engine)

class CImGui_Manager : public CBase
{
private:
	CImGui_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImGui_Manager() = default;

public:
	HRESULT								Initialize(HWND hWnd);
	void								SetUp_Render_ImGui();
	void								EndRender_ImGui();
	vector<float*>						Draw_Gizmo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd, _matrix& matWorld);

	void								Create_DockingSpace();
	void								SetUp_ImGui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);

	void								Set_EndMsg() { m_bEndMsg = true; }

public:
	void								Update_ImGui_Windows(_float fTimeDelta);
	void								Late_Update_ImGui_Windows(_float fTimeDelta);


public:
	void								Render();
private:
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };
	_bool								m_bEndMsg = { false };

	class CImGui_UI*					m_pUI = { nullptr };
	class CImGui_Map*					m_pMap = { nullptr };
	D3D11_VIEWPORT						m_pViewPort;

private:
	void								Release_ImGui();

public:
	static CImGui_Manager*				Create(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	// Base을(를) 통해 상속됨
	void Free() override;
};

END