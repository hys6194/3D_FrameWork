#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
END

BEGIN(Client)

class TP_Camera final : public Camera
{
public:
	typedef struct tagCameraFreeDesc : public Camera::CAMERA_DESC
	{
		_float			fMouseSensor;
	}TP_CAMERA_DESC;
private:
	TP_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	TP_Camera(const TP_Camera& Prototype);
	virtual ~TP_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float					m_fMouseSensor = {};
	_float3					m_vCamEye = { 0.f, 15.f ,-15.f };

	GameObject*				m_pPlayer = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static TP_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual GameObject* Clone(void* pArg);
	virtual void Free() override;
};

END