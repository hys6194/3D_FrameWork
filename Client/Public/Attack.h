#pragma once

#include "Client_Defines.h"
#include "Component.h"

BEGIN(Engine)
class CComponent;
END

BEGIN(Client)

class CAttack final : public CComponent
{
private: 
	CAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAttack(const CAttack& Prototype);
	virtual ~CAttack() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;


public:
	static CAttack*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*			Clone(void* pArg);
	virtual void				Free() override;
};


END