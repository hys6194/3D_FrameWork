#pragma once

#include "Tool_Defines.h"
#include "Camera.h"

BEGIN(Tool)
class CTool_FreeCam final : public CCamera
{
public:
	typedef struct tagCameraFreeDesc : public CCamera::CAMERA_DESC
	{
		_float			fMouseSensor;
	}CAMERA_FREE_DESC;

private:
	CTool_FreeCam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTool_FreeCam(const CTool_FreeCam& Prototype);
	virtual ~CTool_FreeCam() = default;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	_float							m_fMouseSensor = {};

private:
	HRESULT							Ready_Components();

public:
	static CTool_FreeCam*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};

END