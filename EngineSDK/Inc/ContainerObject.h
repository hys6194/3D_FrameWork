#pragma once

#include "GameObject.h"

// 파츠 별로 나눈 게임 오브젝트들을 한꺼번에 모아주는 클래스

BEGIN(Engine)

class ENGINE_DLL CContainerObject : public CGameObject
{
public:
	typedef struct tagContainerObjectDesc : public CGameObject::GAMEOBJECT_DESC
	{
		// 생성할 파츠들의 개수
		_uint iNumPartObjects;
		_uint iState;
	}CONTAINEROBJ_DESC;

protected:
	CContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CContainerObject(const CContainerObject& Prototype);
	virtual ~CContainerObject() = default;


public:
	class CPartObject*				Get_PartObject			(_uint iPartIndex)
	{
		// 해당 파츠가 없을 수도 있잖아? 여기서 예외처리 해주자
		if (nullptr == m_vecParts[iPartIndex])
			return nullptr;

		return m_vecParts[iPartIndex];
	}


public:
	virtual HRESULT					Initialize_Prototype	();
	virtual HRESULT					Initialize				(void* pArg);
	virtual void					Priority_Update			(_float fTimeDelta);
	virtual void					Update					(_float fTimeDelta);
	virtual void					Late_Update				(_float fTimeDelta);
	virtual HRESULT					Render					();

public:
	HRESULT							Add_PartObject			(_uint iPrototypeLevel, const wstring& strPrototypeTag, _uint iPartIndex, void * pArg = nullptr);

protected:
	_uint							m_iNumPartObjects		= {};
	vector<class CPartObject*>		m_vecParts;

public:
	virtual CGameObject*			Clone(void* pArg)		= 0;
	virtual void					Free() override;
};

END