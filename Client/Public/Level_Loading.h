#pragma once

#include "Client_Defines.h"
#include "Level.h"

// 로딩 장면을 보여주는 곳. 배경에 이미지를 띄우고 로딩바가 움직이는 레벨
// 다음 레벨에 대한 자원을 준비한다의 기능을 하는 Loader를 만들어 준다

BEGIN(Client)

class Level_Loading : public Level
{
private:
	Level_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Level_Loading() = default;

public:
	// 레벨 ID를 받아와서 만들어주는 작업을 할 것
	// override 빠짐-> 여기에서 인자값을 새로 받아서 완전히 다른 함수로 변형
	virtual HRESULT Initialize(LEVEL eNextLevelID);
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
private:
	class Loader*		m_pLoader = { nullptr };
	LEVEL				m_eNextLevelID = { LEVEL_END };

public:
	static Level_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID);
	virtual void Free() override;
};

END