#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CCollider;
END


BEGIN(Client)

class CMoloch_Sword final : public CPartObject
{
public:
	typedef struct tagMoloch_SwordtDesc : public CPartObject::PARTOBJ_DESC
	{
		const _float4x4* pHandMatrix;
		const _uint*	 pTargetState	= { nullptr };
		class CMonster*  pOwner			= { nullptr };
	}MOLOCH_SWORD_DESC;

private:
	CMoloch_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMoloch_Sword(const CMoloch_Sword& Prototype);
	virtual ~CMoloch_Sword() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	CShader*					m_pShaderCom		= { nullptr };
	CModel*						m_pModelCom			= { nullptr };

private:
	const _float4x4*			m_pSocketMatrix		= { nullptr };	
	const _float4x4*			m_pHandMatrix		= { nullptr };	
	const _uint*				m_pTargetState		= { nullptr };		//플레이어의 상태를 가져와서 애니메이션의 상태를 변경하려고 함

	class CMonster*				m_pOwner			= { nullptr };

	_float						m_fTotalTime		= { 0.f };

private:
	HRESULT						Bind_SR();
	HRESULT						Ready_Components();

public:
	static CMoloch_Sword*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free() override;
};

END