#pragma once

#include "Component.h"

// 객체의 월드 상태를 표현하는 행렬을 보유하고 있는 객체이다
// 월드에서 변환을 수행하는 다양한 기능의 함수를 담고 있음

BEGIN(Engine)

class ENGINE_DLL Transform final : public Component
{
public:
	typedef struct tagTransform
	{
		_float fSpeedPerSec;
		_float fRotationPerSec;
	}TRANSFORM_DESC;

public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:

	_vector Get_State(STATE eState)
	{
		// 행렬의 각 정보를 의미함
		return XMLoadFloat4x4(&m_f4WorldMatrix).r[eState];
	}

	const _float4x4* Get_WorldMatrix_Ptr() 
	{
		return &m_f4WorldMatrix;
	}

	void Set_State(STATE eState, _vector vState)
	{
		XMStoreFloat4(reinterpret_cast<_float4*>(&m_f4WorldMatrix.m[eState][0]), vState);
	}

private:
	Transform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Transform(const Transform& Prototype);
	virtual ~Transform() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	_float3		Update_Scale();
	HRESULT		Go_Straight(_float fTimeDelta);
	HRESULT		Go_Backward(_float fTimeDelta);
	HRESULT		Go_Right(_float fTimeDelta);
	HRESULT		Go_Left(_float fTimeDelta);
	HRESULT		LookAt(_vector vAt);
	void		Turn(_fvector vAxis, _float fTimeDelta);
	void		Rotation(_fvector vAxis, _float fRadian);

public:
	void SetUp_Scaled(_float fScaleX, _float fScaleY, _float fScaleZ);
	HRESULT Bind_SR(const _char* pConstantName, class Shader* pShader);

private:
	// 객체의 월드 변환을 위한 상태를 가진다
	_float4x4			m_f4WorldMatrix = {};

	_float				m_fSpeedPerSec = {};
	_float				m_fRotationPerSec = {};

public:
	static Transform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;


};

END