#pragma once

#include "GameObject.h"

// 파츠 별로 나눈 게임 오브젝트들을 한꺼번에 모아주는 클래스

BEGIN(Engine)

class ENGINE_DLL ContainerObject : public GameObject
{
public:
	typedef struct tagContainerObjectDesc : public GameObject::GAMEOBJECT_DESC
	{
		// 생성할 파츠들의 개수
		_uint iNumPartObjects;
	}CONTAINEROBJ_DESC;

protected:
	ContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ContainerObject(const ContainerObject& Prototype);
	virtual ~ContainerObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Priority_Update(_float fTimeDelta);
	virtual void	Update(_float fTimeDelta);
	virtual void	Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT			Add_PartObject(_uint iPrototypeLevel, const wstring& strPrototypeTag, _uint iPartIndex, void * pArg = nullptr);

protected:
	_uint						m_iNumPartObjects = {};
	vector<class PartObject*>	m_vecParts;

public:
	virtual GameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END