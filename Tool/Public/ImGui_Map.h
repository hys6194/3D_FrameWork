#pragma once

#include "Tool_Defines.h"
#include "ImGui_Base.h"
#include "Tool_Manager.h"

BEGIN(Engine)
class CGameObject;
class CTransform;
END


BEGIN(Tool)

class CImGui_Map : public CImGui_Base
{
private:
	CImGui_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImGui_Map() = default;

public:
	virtual HRESULT						Initialize();
	virtual void						Update(_float fTimeDelta) override;
	virtual HRESULT						Late_Update(_float fTimeDelta) override;
										
public:
	_bool								Is_Navi() 
	{
		return m_bNavi;
	}

	_bool								Is_Reverse()
	{
		return m_bReverse;
	}

	_bool								Is_Save()
	{
		return m_bSave;
	}

	_bool								Is_Load()
	{
		return m_bLoad;
	}

	list<CGameObject*>*					Get_ObjectList()
	{
		return m_listObject;
	}

public:

	void								Toogle_Save()
	{
		m_bSave = !m_bSave;
	}

	void								Toogle_Load()
	{
		m_bLoad = !m_bLoad;
	}	

	void								Set_Select(_bool bInput)
	{
		m_bSelect = bInput;
	}


public:
	void								Save_MapObjects();
	void								Load_MapObjects();
	void								Set_TransformInfo(class CMap_Object* pObject);

	void								Render_TransformScale();
	void								Render_TransformRotation();
	void								Render_TransformPosition();


private:
	_bool								m_bNavi		= { false };
	_int								m_iObjCnt	= { 0 };		
	_bool								m_bSelect	= { false };
	_bool								m_bAll		= { false };
	_bool								m_bReverse  = { false };

	_bool								m_bSave = { false };
	_bool								m_bLoad = { false };

	class CMap_Object*					m_pObject;
	class CTransform*					m_pTransform;

	list<CGameObject*>*					m_listObject = { nullptr };
private:								
	void								Default_SetButtons(_float fTimeDelta);
	
private:
	void								Button_AddObjects();
	void								Button_DeleleObjects();
	void								Button_NaviCreate();

private:
	void								Button_TransformPosition();


	void								Change_ObjectInfo();
			

public:									
	static CImGui_Map*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void						Free() override;
};

END