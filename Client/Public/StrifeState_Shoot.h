 #pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CStrifeState_Shoot : public CState
{
private	:
	CStrifeState_Shoot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CStrifeState_Shoot() = default;

public:
	virtual HRESULT			Enter_State();
	virtual void			PriorityUpdate_State(_float fTimeDelta);
	virtual void			Update_State(_float fTimeDelta);
	virtual void			LateUpdate_State(_float fTimeDelta);
	virtual HRESULT			Exit_State();

public:
	virtual void			Set_CurAnimation();
	virtual void			Update_Animation(_float fTimeDelta);
	virtual void			Set_PreAnimation();


private:
	CModel*					m_pModelCom;
	_uint					m_iKeyState = {};

private:
	void					Player_ShootMove(_float fTimeDelta);
	void					Player_LookSet(_float fTimeDelta);
	void					Apply_ShootAnimation();

public:
	static CStrifeState_Shoot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual void Free();

	//static CStrifeState_Shoot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iState, GameObject* pOwner);
};

END