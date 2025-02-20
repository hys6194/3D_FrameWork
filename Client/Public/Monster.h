#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class Shader;
class Model;
END

BEGIN(Client)

class Monster final : public GameObject
{
private:
	Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Monster(const Monster& Prototype);
	virtual ~Monster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	Shader* m_pShaderCom = { nullptr };
	Model* m_pModelCom = { nullptr };

private:
	HRESULT Ready_Component();
	HRESULT Bind_SR();

public:
	static  Monster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual GameObject* Clone(void* pArg);
	virtual void Free() override;

};

END
