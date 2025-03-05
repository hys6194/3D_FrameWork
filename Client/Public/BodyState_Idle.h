#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class Shader;
class Model;
END

BEGIN(Client)

class BodyState_Idle : public State
{
private:
	BodyState_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner);
	virtual ~BodyState_Idle() = default;
public:
	virtual HRESULT Enter_State();
	virtual void	Update_State(_float fTimeDelta);
	virtual _float Exit_State();

public:
	Model*			m_pBodyModelCom;
	Shader*			m_pBodyShaderCom;

public:
	static BodyState_Idle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner);
	virtual void Free() override;


};

END