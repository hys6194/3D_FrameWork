#pragma once

/* 특정 레벨에 대한 자원을 준비한다.*/
#include "Client_Defines.h"
#include "Base.h"


// GameInstance 클래스를 왜 가져왔는가? ->  Engine의 기능을 가져와서 사용해야 하기 때문
// 왜? -> 아마 추후에 사용하지 않을까 함
BEGIN(Engine)
class GameInstance;
END

BEGIN(Client)

class Loader final : public Base
{
private:
	Loader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Loader() = default;

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	HRESULT Loading();
	void Show_LoadingState();

public:
	_bool IsFinished() const { return m_isFinished; }

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	GameInstance*				m_pGameInstance = { nullptr };

	// 멀티 쓰레드의 작동을 위해서 필요한 두 멤버 변수임
	HANDLE						m_hThread = {};
	CRITICAL_SECTION			m_CriticalSection = {};

	// 로더에서 어떤 레벨을 읽어올 지 알려주기 위해서 선언
	LEVEL						m_eNextLevelID = { LEVEL_END };

	// 로딩 레벨에서 글자를 읽어오기 위해서 만든 변수
	_tchar							m_szLoading[MAX_PATH] = {};

	// 레벨 전환을 위한 bool 타입의 변수
	_bool							m_isFinished = { false };

public:
	static Loader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID);
	virtual void Free() override;

};

END