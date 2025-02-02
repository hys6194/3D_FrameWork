#pragma once

#include "Client_Defines.h"
#include "Level.h"

// 로딩 씬에서 리소스들을 불러오기 위해 Loader 클래스를 호출하는 레벨(씬)

BEGIN(Client)

class Level_Loading final : public Level
{
private:
	Level_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Level_Loading() = default;

public:
	virtual HRESULT Initialize(LEVEL eLevelID);
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	//_uint			m_iLevelID; -> _uint로 읽어서 가져올 게 아니라 enum으로 받아와야함

	//Loader클래스가 있어야 리소스들을 읽어올 수 있음
	class Loader*		m_pLoader = { nullptr };
	LEVEL				m_eLevelID = {};


public:
	static Level_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID);
	virtual void Free() override;
};

END