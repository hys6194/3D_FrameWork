#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CStrifeState_Dash : public CState
{
private	:
	CStrifeState_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CStrifeState_Dash() = default;

public:
	virtual HRESULT			  Enter_State();
	virtual void			  PriorityUpdate_State(_float fTimeDelta);
	virtual void			  Update_State(_float fTimeDelta);
	virtual void			  LateUpdate_State(_float fTimeDelta);
	virtual HRESULT			  Exit_State();
							  
public:						  
	virtual void			  Set_CurAnimation();
	virtual void			  Update_Animation(_float fTimeDelta);
	virtual void			  Set_PreAnimation();
							  
private:					  
	CModel*					  m_pModelCom = { nullptr };
	_bool					  m_AnimEnd = {};
	_bool					  m_bDashed = { false };
	_uint					  m_iKeyState = {};
	_uint					  m_iCheckDash = {};
							  
private:					  
	void					  Check_KeyInput();
	void					  Set_LastDashAnimation();
	void					  Set_Player_Direction();



public:
	static CStrifeState_Dash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual void Free();
};

END