#pragma once

#include "GameObject.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CFSM : public CComponent
{
protected:
	CFSM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM(const CFSM& Prototype);
	virtual ~CFSM() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg);

public:
	HRESULT Add_State(const _uint& iState, class CState* pState);
	// 
	void	PriUpdate_State(_float fTimeDelta);
	void	Update_State(_float fTimeDelta);
	void	LateUpdate_State(_float fTimeDelta);

	CState*	Find_State(const _uint& iState);
	HRESULT	Delete_State();

public:
	HRESULT Change_State(const _uint& iState);


private:
	map<const _uint, class CState*>	m_mapFSM;
	_uint							m_iCurrentState = {0};
	_uint							m_iPreState = {0};
	CState*							m_pCurrentState;


public:
	static CFSM* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;

};

END