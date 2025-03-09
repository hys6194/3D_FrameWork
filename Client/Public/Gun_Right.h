#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class Shader;
class Model;
END


BEGIN(Client)

class Gun_Right final : public PartObject
{
public:
	typedef struct tagGun_RightDesc : public PartObject::PARTOBJ_DESC
	{
		const _float4x4* pSocketMatrix;
		const _float4x4* pHandMatrix;
		const _uint* pTargetState = { nullptr };
	}WEAPON_DESC;

private:
	Gun_Right(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Gun_Right(const Gun_Right& Prototype);
	virtual ~Gun_Right() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	Shader* m_pShaderCom = { nullptr };
	Model*  m_pModelCom = { nullptr };

private:
	class Player* m_pPlayer = { nullptr };

private:
	const _float4x4* m_pSocketMatrix = { nullptr };	
	const _float4x4* m_pHandMatrix = { nullptr };	

	const _uint* m_pTargetState = { nullptr };		//플레이어의 상태를 가져와서 애니메이션의 상태를 변경하려고 함

private:
	HRESULT Bind_SR();
	HRESULT Ready_Components();


public:
	static Gun_Right* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual GameObject* Clone(void* pArg);
	virtual void Free() override;
};

END