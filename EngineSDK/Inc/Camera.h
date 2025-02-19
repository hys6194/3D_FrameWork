#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL Camera abstract : public GameObject
{
public:
	typedef struct tagCameraDesc : public GameObject::GAMEOBJECT_DESC
	{
		_float3			vEye, vAt;
		_float			fFov, fAspect, fNear, fFar;
	} CAMERA_DESC;

protected:
	Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Camera(const Camera& Prototype);
	virtual ~Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_float					m_fFov = { };
	_float					m_fAspect = { };
	_float					m_fNear = { };
	_float					m_fFar = { };

public:
	virtual GameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END