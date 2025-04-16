#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END


BEGIN(Client)

class CBullet : public CGameObject
{
public:
	typedef struct tagBulletDesc :public GAMEOBJECT_DESC
	{
		_float4x4 f4Hand;
		_float4 fBulletPos;
		_float4 fLook;
	}BULLET_DESC;

private:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& Prototype);
	virtual ~CBullet() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	HRESULT						Ready_Component();
	HRESULT						Bind_SR();

private:
	CShader*					m_pShaderCom = { nullptr };
	CModel*						m_pModelCom = { nullptr };
	CCollider*					m_pColliderCom = { nullptr };

	_bool						m_bDisappear = { false };

	_float4x4					m_matHand;

	_float4						m_fBulletPos;
	_float4						m_fLook;
	_float						m_fSpeed;
	_float						m_fTotalTime = { 0.f };

	_uint						m_iIndex	= { 0 };
	
public:
	static CBullet*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg)override;
	virtual void				Free() override;
};

END