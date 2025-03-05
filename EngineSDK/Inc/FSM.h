#pragma once

#include "GameObject.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL FSM : public Component
{
protected:
	FSM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	FSM(const FSM& Prototype);
	virtual ~FSM() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg);

public:
	HRESULT Add_State(const _uint& iState, class State* pState);
	void	Update_State(_float fTimeDelta);
	State*	Find_State(const _uint& iState);
	HRESULT	Delete_State();

public:
	HRESULT Change_State(const _uint& iState);


private:
	map<const _uint, class State*>	m_mapFSM;
	_uint							m_iCurrentState = {0};
	State*							m_pCurrentState;
	GameObject*						m_pOwner;

	//class Bone*					m_pPreAnimBone;
	//Bone*							m_pNextAnimBone;



public:
	static FSM* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Component* Clone(void* pArg = nullptr);
	virtual void Free() override;

};

END