#pragma once

// 객체들의 원형을 복제한 사본객체들을 오브젝트 매니저에 레벨별로 모아둔다
// 한 레벨 안에서 개발자가 선택한 기준으로 묶어서 보관한다(Layer)

#include "Base.h" 

BEGIN(Engine)

class Object_Manager final : public Base
{
private:
	Object_Manager();
	virtual ~Object_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevel);
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

private:
	_uint								m_iNumLevels = { };
	class GameInstance*					m_pGameInstance = { nullptr };

	map<const _wstring, class Layer*>*	m_pLayers = { nullptr };
	//   map<const _wstring, class Layer*>*
	// = map<const _wstring, list<GameObject*>>

private:
	class Layer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);

public:
	static Object_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END