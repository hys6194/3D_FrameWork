#pragma once

/* 현재 할당된 레벨을 구동한다.(갱신과 렌더?)  */
/* 레벨 교체 시에 이전 레벨을 삭제한다. + 이전 레벨용 자원을 정리한다. */

#include "Base.h"

BEGIN(Engine)

class Level_Manager final : public Base
{
private:
	Level_Manager();
	virtual ~Level_Manager() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

public:
	// 현재 레벨과, 교체할 레벨을 인자값으로 받아와 교체하는 작업을 만들어 줄 것
	HRESULT Change_Level(_uint iLevelIndex, class Level* pNewLevel);

private:
	class GameInstance*		m_pGameInstance = { nullptr };
	class Level*			m_pCurrentLevel = { nullptr };
	_uint					m_iCurrentLevelIndex = {};

public:
	// 싱글톤이기 때문에 static
	static Level_Manager* Create();
	virtual void Free() override;
};

END