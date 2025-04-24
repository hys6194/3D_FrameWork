#pragma once

#include "Base.h"
// 유쌤과의 프로젝트 과정에서 Mgr은 싱글톤으로 관리하고 호출해서 사용하는 방식이었다
// 정쌤은 Mgr을 GameInstance라는 클래스에 Mgr을 모아서 Mgr함수를 내부적으로 호출하는 방식으로 만들었다
// 따라서 Mgr들은 싱글톤으로 제작하지 않는다.

BEGIN(Engine)

class CLevel_Manager : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT						Initialize();
	void						Update(_float fTimeDelta);
	HRESULT						Render();

public:
	HRESULT						Change_Level(_uint iLevelIndex, class CLevel* pNewLevel);

private:
	class CGameInstance*		m_pGameInstance = { nullptr }; // 
	class CLevel*				m_pCurrentLevel = { nullptr }; // 현재 실행중인 레벨(Level 상속받아서 만든 Client의 Level*)
	_uint						m_iCurrentLevelIndex = { };	   // enum LEVEL로 인지할 현재 레벨의 상태값

public:
	static CLevel_Manager*		Create();
	virtual void				Free() override;
};

END