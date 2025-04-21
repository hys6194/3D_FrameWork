#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"


BEGIN(Engine)
class CFSM;
class CNavigation;
class CCollider;
END

BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:
	enum PARTOBJ { PART_BODY, PART_WEAPON, PART_LGUN, PART_RGUN, PART_EFFECT, PART_END };
	enum ANIM_STATE {
		STATE_IDLE			= 0x00000001,
		STATE_RUN			= 0x00000002,
		STATE_JUMP			= 0x00000004,
		STATE_DOUBLEJUMP	= 0x00000008,
		STATE_ATTACK		= 0x00000010,
		STATE_DASH			= 0x00000020,
		STATE_DOUBLEDASH	= 0x00000040,
		STATE_SHOOT			= 0x00000080,
		STATE_NONE			= 0x00000000,
	};
	enum PLAYER_KEYSTATE
	{
		KEY_UP				= 0x00000001,
		KEY_DOWN			= 0x00000002,
		KEY_LEFT			= 0x00000004,
		KEY_RIGHT			= 0x00000008,
		KEY_SPACE			= 0x00000010,
		KEY_SHIFT			= 0x00000020,
		KEY_LB				= 0x00000040,
		KEY_RB				= 0x00000080,
		KEY_NONE			= 0x00000000,
	};
	enum PLAYER_COLLTYPE { COLL_AABB, COLL_OBB, COLL_SPHERE, COLL_END };

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	_uint					Get_PlayerState()
	{
		return m_iState;
	};

	const _uint				Get_PlayerKeyState() const
	{
		return m_iKey;
	};

	_bool					Get_PlayerMove()
	{
		return m_bCanMove;
	};

	void					Set_PlayerState(_uint iState)
	{
		m_iState = iState;
	}

	void					Insert_KeyState(_uint iState)
	{
		m_iKey |= iState;
	}

	void					Delete_KeyState(_uint iState)
	{
		m_iKey &= ~iState;
	}

	void					Set_PlayerMove(_bool bMove)
	{
		m_bCanMove = bMove;
	}

	_bool					Is_Hit()
	{
		return m_bHit;
	}

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

private:
	_uint					m_iState			= { STATE_NONE };
	_uint					m_iKey				= { KEY_DOWN };


	_bool					m_bIsDashed			= { false };
	_bool					m_bCanMove			= { true };
	_bool					m_bHit				= { false };

	_float					m_fTotalTime = { 0.f };

private:
	CFSM*					m_pFSMCom					= { nullptr };
	CCollider*				m_pColliderCom[COLL_END]	= { nullptr };
	CNavigation*			m_pNavigationCom			= { nullptr };
	class CStatus*			m_pStatusCom				= { nullptr };

private:
	HRESULT					Ready_Components();
	HRESULT					Ready_PartObjects();
	HRESULT					Ready_States();
	HRESULT					Bind_SR();

	void					Calculate_StatusInfo();

private:
	void					Input_Keys();

public:
	static CPlayer*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;
};

END