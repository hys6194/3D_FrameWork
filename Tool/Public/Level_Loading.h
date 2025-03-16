#pragma once

#include "Tool_Defines.h"
#include "Level.h"

BEGIN(Tool)

class Level_Loading final : public CLevel
{
private:
	Level_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Level_Loading() = default;

public:
	virtual HRESULT					Initialize(LEVEL eLevelID);
	virtual void					Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;


private:
	class Loader*					m_pLoader = { nullptr };
	LEVEL							m_eLevelID = {};


public:
	static Level_Loading*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID);
	virtual void					Free() override;
};

END