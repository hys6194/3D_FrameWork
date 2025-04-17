#pragma once

#include "Component.h"

// 객체의 월드 상태를 표현하는 행렬을 보유하고 있는 객체이다
// 월드에서 변환을 수행하는 다양한 기능의 함수를 담고 있음

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct tagTransform
	{
		_float fSpeedPerSec;
		_float fRotationPerSec;
	}TRANSFORM_DESC;

public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POS, STATE_END };

public:

	_vector Get_Scale()
	{
		return XMVectorSet(m_f4WorldMatrix._11,
			m_f4WorldMatrix._22,
			m_f4WorldMatrix._33,
			m_f4WorldMatrix._44);
	}

	_vector Get_State(STATE eState)
	{
		// 행렬의 각 정보를 의미함
		return XMLoadFloat4x4(&m_f4WorldMatrix).r[eState];
	}

	const _float4x4* Get_WorldMatrix_Ptr()
	{
		return &m_f4WorldMatrix;
	}

public:

	void Set_Matrix(const _float4x4* _fMatrix)
	{
		m_f4WorldMatrix = *_fMatrix;
	}

	void Set_State(STATE eState, _vector vState)
	{
		XMStoreFloat4(reinterpret_cast<_float4*>(&m_f4WorldMatrix.m[eState][0]), vState);
	}

	void Set_RotationSpeed(_float fRadian)
	{
		m_fRotationPerSec = fRadian;
	}

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;

public:
	_float3						Update_Scale();
	HRESULT						Go_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT						Go_Backward(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT						Go_Right(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT						Go_Left(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT						Jump(_float fTimeDelta);
	HRESULT						LookAt(_vector vAt);
	HRESULT						Dash(_float4 fDelta, class CNavigation* pNavigation = nullptr, _float fMag = 1.f);
	HRESULT						Avoid(_float4 fDelta, class CNavigation* pNavigation = nullptr);
	void						Turn(_fvector vAxis, _float fTimeDelta);
	_bool						Turn_ToTarget(_fvector vAxis, _float fTimeDelta, _vector vTargetToDir, _bool bClamp = false);
	void						Rotation(_fvector vAxis, _float fRadian);

public:
	HRESULT						Move_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT						Move_Backward(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT						Move_Right(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT						Move_Left(_float fTimeDelta, class CNavigation* pNavigation = nullptr);

	HRESULT						Move_Left_Up(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT						Move_Right_Up(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT						Move_Left_Down(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	HRESULT						Move_Right_Down(_float fTimeDelta, class CNavigation* pNavigation = nullptr);

public:
	void						SetUp_Scaled(_float fScaleX, _float fScaleY, _float fScaleZ);
	HRESULT						Bind_SR(const _char* pConstantName, class CShader* pShader);

private:
	// 객체의 월드 변환을 위한 상태를 가진다
	_float4x4					m_f4WorldMatrix = {};

	_float						m_fSpeedPerSec = {};
	_float						m_fRotationPerSec = {};

	_bool						m_bTurn = { false };

public:
	static CTransform*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*			Clone(void* pArg) override;
	virtual void				Free() override;


};

END