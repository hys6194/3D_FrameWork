#pragma once

// 객체들의 원형을 복제한 사본객체들을 오브젝트 매니저에 레벨별로 모아둔다
// 한 레벨 안에서 개발자가 선택한 기준으로 묶어서 보관한다(Layer)

#include "Base.h"


BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT									Initialize(_uint iNumLevel);
	HRESULT									Add_GameObject(_uint iPrototypeLevelIndex, const wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg);
	void									Priority_Update(_float fTimeDelta);
	void									Update(_float fTimeDelta);
	void									Late_Update(_float fTimeDelta);
	void									Clear(_uint iLevelIndex);

public:
	class CLayer*							Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
	class CGameObject*						Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, const _tchar* strObjectTag);

private:
	_uint									m_iNumLevels = { };
	class CGameInstance*					m_pGameInstance = { nullptr };

	map<const _wstring, class CLayer*>*		m_pLayers = { nullptr };
	//   map<const _wstring, class Layer*>*
	// = map<const _wstring, list<CGameObject*>>

public:
	static CObject_Manager*					Create(_uint iNumLevels);
	virtual void							Free() override;
};

END