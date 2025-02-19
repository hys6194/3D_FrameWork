#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
END

BEGIN(Client)

class Camera_Free final : public Camera
{
public:
	typedef struct tagCameraFreeDesc : public Camera::CAMERA_DESC
	{
		_float			fMouseSensor;
	}CAMERA_FREE_DESC;
private:
	Camera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Camera_Free(const Camera_Free& Prototype);
	virtual ~Camera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float					m_fMouseSensor = {};

private:
	HRESULT Ready_Components();	

public:
	static Camera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual GameObject* Clone(void* pArg);
	virtual void Free() override;
};

END