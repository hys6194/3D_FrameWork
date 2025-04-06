#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT							Initialize();
	void							Update(_float fTimeDelta);
	HRESULT							Render();	

private:
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };
	CGameInstance*					m_pGameInstance = { nullptr };


#ifdef _DEBUG
private:
	_float							m_fTimeAcc = { 0.f };
	_tchar							m_szFPS[MAX_PATH] = {};
	_uint							m_iNumDraw = { };
#endif
	_uint							m_iCurLevel = { LEVEL_LOGO };

private:
	HRESULT							Start_Level(LEVEL eLevelID);
	HRESULT							Ready_Fonts();
	HRESULT							Ready_Gara();

public:
	static CMainApp*				Create();
	virtual void					Free() override;
};

END