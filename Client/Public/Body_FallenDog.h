#pragma once

#include "Client_Defines.h"
#include "Body_Monster.h"

BEGIN(Engine)
class CShader;
class CModel;
END

//추후 보스 몬스터 혹은 일반 몬스터가 무기를 들 수 있으므로
BEGIN(Client)


class CBody_FallenDog final : public CBody_Monster
{

private:
	CBody_FallenDog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_FallenDog(const CBody_FallenDog& Prototype);
	virtual ~CBody_FallenDog() = default;

public:
	virtual HRESULT								Initialize_Prototype() override;
	virtual HRESULT								Initialize(void* pArg) override;
	virtual void								Priority_Update(_float fTimeDelta) override;
	virtual void								Update(_float fTimeDelta) override;
	virtual void								Late_Update(_float fTimeDelta) override;
	virtual HRESULT								Render() override;

private:
	HRESULT										Ready_Component();
	HRESULT										Ready_SocketMatrices();
	HRESULT										Bind_SR();

public:
	// CPartObject을(를) 통해 상속됨
	static CBody_FallenDog*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*						Clone(void* pArg);
	virtual void								Free() override;
};

END
