#pragma once

#include "Transform.h"

BEGIN(Engine)

// 왜 ENGINE_DLL -> 게임 오브젝트들을 실질적으로 만드는 곳은 Client라서 DLL 내보내기 해야함

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagGameObjectDesc : public CTransform::TRANSFORM_DESC
	{
		// 오브젝트의 이름
		_tchar			szGameObjectTag[MAX_PATH];
	}GAMEOBJECT_DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	_bool Compare_Name(const _tchar* strObjectTag) const
	{
		return !lstrcmp(m_szGameObjectTag, strObjectTag);
	};

public:
	CTransform* Get_Transform()
	{
		return m_pTransformCom;
	};


protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	class CGameInstance*			m_pGameInstance = { nullptr };	
	CTransform*					m_pTransformCom = { nullptr };
	_tchar						m_szGameObjectTag[MAX_PATH] = {};


protected:
	map<const _wstring, class CComponent*>			m_mapComponent;

protected:
	// 왜 GameObject에 함수를 선언? 
	// map으로 Component를 담고있는 멤버 변수가 GameObject에 존재
	// 이에, 쉽게 추가 및 검색 기능을 한꺼번에 처리하려고 GameObject에 추가
	// 왜 Component**형?
	// 
	// 
	HRESULT Add_Component(_uint iLevelIndex, const _wstring& strPrototypeTag,
		CComponent** ppOut, const _wstring& strComponentTag, void* pArg = nullptr);

private:
	HRESULT Set_TransformCom(void* pArg);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END