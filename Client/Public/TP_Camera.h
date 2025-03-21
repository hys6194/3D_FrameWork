#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CTP_Camera final : public CCamera
{
public:
	typedef struct tagCameraFreeDesc : public CCamera::CAMERA_DESC
	{
		_float			fMouseSensor;
	}TP_CAMERA_DESC;
private:
	CTP_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTP_Camera(const CTP_Camera& Prototype);
	virtual ~CTP_Camera() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	_float						m_fMouseSensor = {};
	_float3						m_vCamEye = { 0.f, 15.f ,-12.5f };

	CGameObject*				m_pPlayer = { nullptr };

private:
	HRESULT						Ready_Components();

public:
	static CTP_Camera*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free() override;
};

END