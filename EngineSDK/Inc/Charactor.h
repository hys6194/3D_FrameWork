#pragma once

#include "GameObject.h"

class FSM;

BEGIN(Engine)

class ENGINE_DLL Charactor : public GameObject	
{
protected:
	Charactor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Charactor(const Charactor& Prototype);
	virtual ~Charactor() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	FSM*				m_pFSMCom;

private:
	HRESULT Ready_FSM();

public:
	virtual GameObject* Clone(void* pArg);
	virtual void Free() override;
};

END