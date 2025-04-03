#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CStrifeState_Idle : public CState
{
private:
	CStrifeState_Idle(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CStrifeState_Idle() = default;
public:
	virtual HRESULT Enter_State();
	virtual void PriorityUpdate_State(_float fTimeDelta);
	virtual void Update_State(_float fTimeDelta);
	virtual void LateUpdate_State(_float fTimeDelta);
	virtual HRESULT Exit_State();

public:
	virtual void Set_CurAnimation();
	virtual void Update_Animation(_float fTimeDelta);
	virtual void Set_PreAnimation();

private:
	CModel*			m_pModelCom = { nullptr };
	_uint			m_iKeyState = { 0 };

private:
	void		 Check_KeyInput();

public:
	static CStrifeState_Idle* Create(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual void Free() override;


};

END