#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CCollider;
END


BEGIN(Client)

class CFist_Right final : public CPartObject
{
public:
	typedef struct tagFist_RightDesc : public CPartObject::PARTOBJ_DESC
	{
		const _float4x4* pSocketMatrix;
		const _float4x4* pHandMatrix;
		const _uint* pTargetState = { nullptr };
		class CMonster* pOwner = { nullptr };
	}FIST_RIGHT_DESC;

private:
	CFist_Right(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFist_Right(const CFist_Right& Prototype);
	virtual ~CFist_Right() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	CShader*					m_pShaderCom = { nullptr };
	CModel*						m_pModelCom = { nullptr };
	CCollider*					m_pColliderCom = { nullptr };

private:
	const _float4x4*			m_pSocketMatrix = { nullptr };	
	const _float4x4*			m_pHandMatrix = { nullptr };	

	const _uint*				m_pTargetState = { nullptr };		//플레이어의 상태를 가져와서 애니메이션의 상태를 변경하려고 함

	class CMonster*				m_pOwner = { nullptr };


	_float						m_fCool = {};
	_bool						m_bShoot = { false };
	_float						m_fTotalTime = {0.f};

private:
	HRESULT						Bind_SR();
	HRESULT						Ready_Components();


public:
	static CFist_Right*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free() override;
};

END