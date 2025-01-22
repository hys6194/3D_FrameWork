#pragma once
#include "Base.h"

class Component;

class ENGINE_DLL Transform final : public Component
{
public:
	typedef struct tagTransform
	{
		_float fSpeedPerSec;
		_float fRotationPerSec;

	}TRANSFORM_DESC;

public:
	enum STATE {STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POS, STATE_END};

public:
	_vector Get_State(STATE eState)
	{
		return XMLoadFloat4x4(&m_matWorld).r[eState];
	}

	void Set_State(STATE eState, _vector vState)
	{
		//_matrix matWorld = XMLoadFloat4x4(&m_matWorld);
		//matWorld.r[eState] = vState;
		//XMStoreFloat4x4(&m_matWorld, matWorld);


		//위의 3줄을 한줄로 줄인 것인데, 
		XMStoreFloat4(reinterpret_cast<_float4*>(& m_matWorld.m[eState][0]), vState);
	}


private:
	Transform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual  ~Transform();

public:
	HRESULT Initialize_Prototype()override;
	HRESULT Initialize(void* pArg)override;

public:
	HRESULT Move_Forward();
	HRESULT Move_Back();
	HRESULT Move_Left();
	HRESULT Move_Right();

private:
	_float4x4 m_matWorld = {};

	_float m_fSpeedPerSec = {};
	_float m_fRotationPerSec = {};

public:
	static Transform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;



};

