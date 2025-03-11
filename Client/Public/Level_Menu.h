#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Menu final : public CLevel
{
private:
	CLevel_Menu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Menu() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Layer_LogoPlayer(const _tchar* pLayerTag);

private:
	vector<HWND>	m_vecVidio;

public:
	static CLevel_Menu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END