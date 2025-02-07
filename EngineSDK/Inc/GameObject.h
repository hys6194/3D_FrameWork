#pragma once

#include "Base.h"

BEGIN(Engine)

class Transform
{
public:
	typedef struct tagTransformDesc
	{
		_float			fSpeedPerSec;
		_float			fRotationPerSec;
	}TRANSFORM_DESC;

};

// 왜 ENGINE_DLL -> 게임 오브젝트들을 실질적으로 만드는 곳은 Client라서 DLL 내보내기 해야함

class ENGINE_DLL GameObject abstract : public Base
{
public:
	typedef struct tagGameObjectDesc : public Transform::TRANSFORM_DESC
	{
		// 오브젝트의 이름
		_tchar			szGameObjectTag[MAX_PATH];
	}GAMEOBJECT_DESC;

protected:
	GameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	GameObject(const GameObject& Prototype);
	virtual ~GameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();


protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	class GameInstance*			m_pGameInstance = { nullptr };

	_tchar						m_szGameObjectTag[MAX_PATH] = {};

public:
	virtual GameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END