#pragma once

#include "Base.h"
#include "Client_Defines.h"

// 멀티스레드를 생성하여 오브젝트 생성 시 필요한 리소스들을 읽어들이기 위한 클래스
// LEVEL enum을 통해서 어떤 레벨을 읽어들일 지 결정


class Loader : public Base
{
private:
	Loader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Loader() = default;

public:
	HRESULT Initialize(LEVEL eNextLevelID);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	HANDLE							m_hThread = {};
	CRITICAL_SECTION* m_pCriticalSection = { nullptr };
	LEVEL							m_eNextLevelID = { LEVEL_END };

public:
	static Loader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID);
	virtual void Free() override;
};

