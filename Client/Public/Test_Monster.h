#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CTest_Monster final : public CGameObject
{
public:
	enum COLLIDERTYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
private:
	CTest_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTest_Monster(const CTest_Monster& Prototype);
	virtual ~CTest_Monster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CCollider* m_pColliderCom[TYPE_END] = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

	_uint	 m_iAnimIndex = {};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CTest_Monster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END