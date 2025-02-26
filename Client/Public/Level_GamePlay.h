#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class Level_GamePlay final : public Level
{
private:
	Level_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Level_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Lights();

public:
	static Level_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END