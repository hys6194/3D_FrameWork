#pragma once

#include "Tool_Defines.h"
#include "Level.h"

BEGIN(Tool)

class Tool_Level : public CLevel
{
private:
	Tool_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Tool_Level() = default;

public:
	virtual HRESULT					Initialize() override;
	virtual void					Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;


public:
	HRESULT							Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT							Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT							Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT							Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT							Ready_Lights();

public:
	static Tool_Level*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void					Free() override;
};

END