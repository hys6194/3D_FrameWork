#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END


BEGIN(Client)

class CGun_Left final : public CPartObject
{
public:
	typedef struct tagGun_LeftDesc : public CPartObject::PARTOBJ_DESC
	{
		const _float4x4*			pSocketMatrix;
		const _float4x4*			pHandMatrix;
		const _uint*				pTargetState = { nullptr };
		class CPlayer*				pOwner = { nullptr };
	}WEAPON_DESC;

private:
	CGun_Left(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGun_Left(const CGun_Left& Prototype);
	virtual ~CGun_Left() = default;

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

private:
	_float						m_fTotalTime = { 0 };

private:
	const _float4x4*			m_pSocketMatrix = { nullptr };	// 특정 뼈에 붙이려고 하는 소켓의 매트릭스
	const _float4x4*			m_pHandMatrix = { nullptr };	// 특정 뼈에 붙이려고 하는 소켓의 매트릭스
	const _uint*				m_pTargetState = { nullptr };	//플레이어의 상태를 가져와서 애니메이션의 상태를 변경하려고 함

	_float						m_fCool = {};
	_bool						m_bShoot = { false };
	class CPlayer*				m_pOwner = { nullptr };

private:
	HRESULT						Bind_SR();
	HRESULT						Ready_Components();

	HRESULT						Create_Bullet();

public:
	static CGun_Left*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free() override;
};

END