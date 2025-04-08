#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class CFSM;
class CNavigation;
class CCollider;
END

BEGIN(Client)

class CMonster : public CContainerObject
{
protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	typedef struct tagMonsterInfo : public CGameObject::GAMEOBJECT_DESC
	{
		_bool bBoss = { false };
		_bool bWave = { false };
		_uint iState = { STATE_IDLE };
		_uint iHP = {};

		_float fDetectDistance = { 15.f };
		_float fNoticeDistance = { 15.f };

	}MONSTER_DESC;


	enum PARTOBJ { PART_BODY, PART_LEFT, PART_RIGHT, PART_EFFECT, PART_END };

public:
	enum MONSTER_STATE { STATE_IDLE, STATE_SEARCH, STATE_TRACE, STATE_HIT, STATE_ATTACK, STATE_AVOID, STATE_DEAD, STATE_NONE };

public:
	void									Set_Dead			(_bool bDead)			{ m_bIsDead		= bDead; }
	void									Set_Hit				(_bool bHit)			{ m_bHit		= bHit;  }
	void									Set_Rec				(_bool bRec)			{ m_bRec		= bRec;  }
	void									Set_Critical		(_bool bCri)			{ m_bCri		= bCri;  }
	void									Set_PreState		(_uint iState)			{ m_iPreState	= iState;}

public:
	//Getter
	_bool									Is_Dead()									{ return m_bIsDead; }
	_bool									Is_Hit ()									{ return m_bHit;    }
	_bool									Is_Rec ()									{ return m_bRec;    }
	_bool									Is_Critical()								{ return m_bCri;    }

	_uint									Get_PreState()								{ return m_iPreState;}
	_uint									Get_Index()									{ return m_iIndex;}
public:
	void									Change_CurrentState (MONSTER_STATE eState)  { m_iState = eState; }

public:
	virtual HRESULT							Initialize_Prototype()						override;
	virtual HRESULT							Initialize(void* pArg)						override;
	virtual void							Priority_Update(_float fTimeDelta)			override;
	virtual void							Update(_float fTimeDelta)					override;
	virtual void							Late_Update(_float fTimeDelta)				override;
	virtual HRESULT							Render()									override;


public:
	virtual HRESULT							Ready_PartObjects()							= 0;
	virtual HRESULT							Ready_Components();

protected:
	CFSM*									m_pFSMCom									= { nullptr };
	CNavigation*							m_pNavigationCom							= { nullptr };
	CCollider*								m_pColliderCom								= { nullptr };


protected:
	_bool									m_bIsDead									= { false };
	_bool									m_bHit										= { false };
	_bool									m_bRec										= { false };
	_bool									m_bCri										= { false };
	_bool									m_bIsBoss									= { false };
	_bool									m_bWave										= { false };

	_uint									m_iPreState									= { STATE_NONE};
	_uint									m_iState									= { STATE_NONE };
	_uint									m_iHP										= {};
	_uint									m_iIndex									= { };

	_float									m_fNoticeDistance							= {};
	
	_wstring								m_strModelTag								= {};

protected:
	HRESULT									Bind_SR();

public:
	virtual CGameObject*					Clone(void* pArg)							= 0;
	virtual void							Free()										override;

};

END
