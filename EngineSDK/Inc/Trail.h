#pragma once

#include "Component.h"

BEGIN (Engine)

class ENGINE_DLL CTrail abstract : public CComponent
{
public:
	typedef struct tagTrailDesc
	{
		_float2 fTime; // x = Create_Time, y = Life_Time
		_float2 fSize;
		_float4 fColor;
		_float4 fDir;
	}TRAIL_DESC;

protected:
	CTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTrail() = default;

public:
	virtual HRESULT			Initialize_Prototype() = 0;
	virtual HRESULT			Initialize(void* pArg) = 0;
	virtual void			Priority_Update(_float fTimeDelta) = 0;
	virtual void			Update(_float fTimeDelta) = 0;
	virtual void			Late_Update(_float fTimeDelta) = 0;
	virtual HRESULT			Render() = 0;

protected:
	_float2					m_fBufferTime	= {};
	_float2					m_fSize			= {};
	_float4					m_fColor		= {};
	_float4					m_fDir			= {};

public:
	virtual void			Free() override;
};

END