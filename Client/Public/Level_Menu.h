#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class Level_Menu final : public Level
{
private:
	Level_Menu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Level_Menu() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Layer_LogoPlayer(const _tchar* pLayerTag);

private:
	vector<HWND>	m_vecVidio;

public:
	static Level_Menu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END