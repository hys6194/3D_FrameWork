#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class Shader;
class Model;
END

BEGIN(Client)

class StrifeState_Run : public State
{
private:
	StrifeState_Run(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner);
	virtual ~StrifeState_Run() = default;
public:
	virtual HRESULT Enter_State();
	virtual void	Update_State(_float fTimeDelta);
	virtual HRESULT Exit_State();

private:
	Model*			m_pModelCom;
	Shader*			m_pBodyShaderCom;

public:
	static StrifeState_Run* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner);
	virtual void Free() override;


};

END