#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class CFSM;
class CNavigation;
class CCollider;
END

BEGIN(Client)

class CMonster final : public CContainerObject
{
public:
	typedef struct tagMonsterInfo : public CGameObject::GAMEOBJECT_DESC
	{
		_bool bWave{};
		_uint iState{};
		_uint iHP{};

	}MONSTER_DESC;

private:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

	virtual HRESULT				Ready_PartObjects();	
private:
	CFSM*						m_pFSMCom			= { nullptr };
	CNavigation*				m_pNavigationCom	= { nullptr };
	CCollider*					m_pColliderCom		= { nullptr };


private:

	_bool						m_bIsDead			= { false };
	_bool						m_bHit				= { false };

	_uint						m_iState			= { STATE_NONE };

private:
	HRESULT						Ready_Components();
	HRESULT						Ready_States();
	HRESULT						Bind_SR();

public:
	static  CMonster*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free() override;

};

END
