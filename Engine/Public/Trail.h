#pragma once

#include "GameObject.h"

BEGIN (Engine)

class ENGINE_DLL CTrail abstract : public CGameObject
{
public:
	typedef struct tagTrailDesc
	{
		TRAIL_TYPE eType;
		_uint	   iLevelIndex;
		_float2    fTime;  // x = Create_Time, y = Life_Time
		_float2    fSize;
		_float3    fDir;	//카메라를 바라볼 지, 아니면 나아가는 방향으로 할지
		_float3    fPos;	
		_float4    fColor;
		_wstring   strShaderTag;
		_wstring   strVIBufferTag;

		CGameObject* pObject;
	}TRAIL_DESC;

protected:
	CTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrail(const CTrail& Prototype);
	virtual ~CTrail() = default;

public:
	virtual HRESULT			Initialize_Prototype() = 0;
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta) = 0;
	virtual void			Update(_float fTimeDelta) = 0;
	virtual void			Late_Update(_float fTimeDelta) = 0;
	virtual HRESULT			Render() = 0;

	virtual HRESULT			Bind_SR() = 0;

protected:
	TRAIL_TYPE				m_eType			 = {};
	_uint					m_iLevelIndex	 = {};
	_float2					m_fBufferTime	 = {};
	_float2					m_fSize			 = {};
	_float3					m_fDir			 = {};
	_float3					m_fPos			 = {};
	_float4					m_fColor		 = {};
	_wstring				m_strShaderTag	 = {};
	_wstring				m_strVIBufferTag = {};

	CGameObject*			m_pOwner = { nullptr };

	class CShader*			m_pShaderCom	= { nullptr };

public:
	virtual void			Free() override;
};

END