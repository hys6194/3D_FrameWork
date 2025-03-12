#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public CGameObject::GAMEOBJECT_DESC
	{
		_float3			vEye;			// 카메라 위치
		_float3			vAt;			// 카메라 바라보는 방향
		_float			fFov;			// 카메라 시야각
		_float			fAspect;		// 카메라 가로세로 비율 
		_float			fNear;			// 카메라 근평면
		_float			fFar;			// 카메라 원평면
	} CAMERA_DESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT Renew_Matrices();

protected:
	_float					m_fFov = { };
	_float					m_fAspect = { };
	_float					m_fNear = { };
	_float					m_fFar = { };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END