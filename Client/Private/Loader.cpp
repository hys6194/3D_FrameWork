#include "Loader.h"
#include "GameInstance.h"

Loader::Loader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ GameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}
unsigned int APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	// pArg를 인자로 왜 받아옴?
	// 다중 쓰레드 환경에서 호출될 가능성이 높아서, 해당 클래스에서 특정 데이터를 전달받아
	// 작업을 수행하기 위해 사용
	// 
	// 주요 목적 
	// 일반적으로 스레드 함수는 고정된 서명(보통 void* 타입의 단일 매개변수)을 가집니다.
	// 
	// 사용자 정의 데이터를 전달하려면 이 매개변수를 통해 데이터의 포인터를 전달하고, 
	// 함수 내부에서 적절한 타입으로 변환하여 사용해야 합니다.
	// 
	// Loader 객체를 전달하여 LoadingMain 함수에서 이 객체의 메서드와 데이터를 활용할 수 있습니다.
	// 
	// void* 의 선언 이유 : 낮은 수준의 C스타일 인터페이스에서 데이터 타입의 안전성을
	// 유지하기 위해서 void* 을 사용하는 것.
	// 하지만 실제로 작업하기 위해선, static_cast를 통해 Loader* 타입으로 변환해야 함
	// 
	// 왜 static_cast? 
	// 컴파일 타임에 타입변환을 확인하면서 Loader* 타입임을 알고 있으므로 안전한 변환.
	// 
	//
	Loader* pLoader = static_cast<Loader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;


	// 흐름 : 외부 코드에서 스레드를 생성할 때, pArg에 특정 객체(예: Loader)를 전달합니다.
	// 함수 내부에서 pArg를 적절한 타입으로 변환하여 작업에 사용합니다

	// 정리 : 외부에서 데이터 전달 -> 스레드 함수 내에서 데이터 변환
}


HRESULT Loader::Initialize(LEVEL eNextLevelID)
{
	/* 멤버 변수에 받아온 특정 레벨을 대입하면서*/
	/* 어떤 레벨에 대해서 준비해야 하는지 알게되었다. */
	/* Create를 하면서 Initialize 호출해서 그런거임. 잊지 마셈*/
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	/* 추가 스레드(로딩스레드)를 만든다. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT Loader::Loading()
{
	return S_OK;
}

void Loader::Show_LoadingState()
{
	//로딩 레벨에서 어떤 자원을 가져오고 있는지 출력하기 위해 만든 함수
	// Loading~~XX 라는 함수를 만들어서 로딩 레벨에서 출력해줄 것
	SetWindowText(g_hWnd, m_szLoading);
}
Loader* Loader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	Loader* pInstance = new Loader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : Loader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Loader::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pGameInstance);
}
