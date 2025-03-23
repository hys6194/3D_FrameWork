#pragma once

#include "Tool_Defines.h"
#include "ImGui_Base.h"

BEGIN(Tool)

class CImGui_Map : public CImGui_Base
{
private:
	CImGui_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImGui_Map() = default;

public:
	virtual HRESULT						Initialize() override;
	virtual void						Update(_float fTimeDelta) override;
	virtual HRESULT						Late_Update(_float fTimeDelta) override;
										
			
private:
	_int								m_iObjCnt = { 0 };
										
private:								
	void								Default_SetButtons(_float fTimeDelta);
	
private:
	void								Button_AddObjects();
	void								Button_DeleleObjects();
	void								Button_NaviCreate();

private:
	_bool								m_bNavi = { false };
			

public:									
	static CImGui_Map*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void						Free() override;
};

END