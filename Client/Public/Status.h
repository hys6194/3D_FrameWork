#pragma once

#include "Client_Defines.h"
#include "Component.h"

BEGIN(Engine)
class CComponent;
END

BEGIN(Client)

class CStatus final : public CComponent
{
public:
	typedef struct tagStatusDesc
	{
		_float iHP;
		_float iAttack;
		class CGameObject* pOwner; // 일단 들고있게 하자
	}STATUS_DESC;

private:
	CStatus(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStatus(const CStatus& Prototype);
	virtual ~CStatus() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);

public:
	const STATUS_DESC&  Get_StatusDesc() const { return m_tDesc; }
	void				Set_StatusDesc(const STATUS_DESC& StatusDesc) { m_tDesc = StatusDesc; }

	const STATUS_DESC&  Get_MaxStatusDesc() const { return m_tMaxDesc; }
	void				Set_MaxStatusDesc(const STATUS_DESC& MaxStatusDesc) { m_tMaxDesc = MaxStatusDesc; }

public:
	_bool				Is_Damaged() const { return m_bIsDamaged; }
	void				Reset_Damaged() { m_bIsDamaged = false; }

	void				Take_Damage(_float iDamage);
	void				Take_Heal(_float iHeal) { m_tDesc.iHP = Clamp(m_tDesc.iHP + iHeal, 0.f, 100.f); }

public:
	void				Calculate_StatusInfo(_int iAttack);

public:
	STATUS_DESC			m_tDesc			{};
	STATUS_DESC			m_tMaxDesc		{};

	_bool				m_bIsDamaged	= { false };

	class CGameObject*  m_pOwner		= { nullptr };


public:
	static CStatus*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void		Free() override;
};

END