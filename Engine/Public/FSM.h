#pragma once

#include "Component.h"


BEGIN(Engine)

class FSM : public Component
{
protected:
	FSM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~FSM() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg);

public:
	HRESULT Add_State(const _wstring& strStateTag, class State* pState);
	HRESULT Find_State(const _wstring& strStateTag);
	HRESULT Delete_State();


private:
	map<const _int, class State*> m_mapFSM;
	vect9<State*>					m_vecFSM;

public:
	static FSM* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Component* Clone(void* pArg);
	virtual void Free() override;

};

END