#pragma once

#include "Base.h"

// GameObject를 통해서 만들고 이를 Layer 클래스에 저장
// Object들을 Rendering 하기 위해서 만든 클래스 
// 왜 Layer로 구분? -> 오브젝트끼리의 상호작용을 편하게 하기 위해서

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

public:
	class CGameObject*					Find_Object(const _tchar* strObjectTag);
	HRESULT								Delete_LastObject();
	class CComponent*					Find_Component(const _wstring& strComponentTag, _uint iIndex = 0);
	list<class CGameObject*>*			Get_GameObjectList()
	{
		return &m_GameObjects;
	}

private:
	// 특정 게임 오브젝트들은 키 값으로 사용할 것
	// 한 배열당 여러 객체들을 가지고 있음
	list<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END
