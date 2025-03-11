#pragma once

#include "GameObject.h"

// 파츠로 구분할 게임 오브젝트에게 상속을 부여하는 클래스


BEGIN(Engine)

class ENGINE_DLL PartObject : public CGameObject
{
public:
	typedef struct tagPartObjectDesc : public CGameObject::GAMEOBJECT_DESC
	{
		const _float4x4* pParentMatrix = { nullptr };
	}PARTOBJ_DESC;

protected:
	PartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	PartObject(const PartObject& Prototype);
	virtual ~PartObject() = default;

public:	
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Priority_Update(_float fTimeDelta);
	virtual void	Update(_float fTimeDelta);
	virtual void	Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	const _float4x4*		m_pParentMatrix = { nullptr };
	_float4x4				m_CombinedWorldMatrix = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END