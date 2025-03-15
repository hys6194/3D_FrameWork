#pragma once

#include "Base.h"
#include "Tool_Defines.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Tool)

class MainApp : public CBase
{
private:
	MainApp();
	virtual ~MainApp() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };

#ifdef _DEBUG
private:
	_float							m_fTimeAcc = { 0.f };
	_tchar							m_szFPS[MAX_PATH] = {};
	_uint							m_iNumDraw = { };
	_uint							m_iCurLevel = { LEVEL_LOGO };
#endif

private:
	HRESULT Ready_ImGui();

public:
	static MainApp* Create();
	virtual void Free() override;

};

END