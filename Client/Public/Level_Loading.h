#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* Level_Loading클래스는 로딩창을 띄웠을 때 보여지는 화면을 보여주기 위한 클래스이다 */

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
	// 다음 레벨을 준비하기 위해선 로더가 필요하다?
	// Loading클래스가 Loader를 이용해서 불러오고자 하는 레벨을 불러올 것이기 때문
	class Loader*		m_pLoader = { nullptr };
	LEVEL				m_eNextLevelID = { LEVEL_END };

public:
	static Level_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID);
	virtual void Free() override;
};

END