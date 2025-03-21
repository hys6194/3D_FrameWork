#include "MainApp.h"
#include "GameInstance.h"

#include "Level_Loading.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	// 엔진 사용준비
	ENGINE_DESC			EngineDesc{};

	EngineDesc.hInstance = g_hInstance;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.isWindowed = true;
	EngineDesc.iWidth_VP = g_iWinSizeX;
	EngineDesc.iHeight_VP = g_iWinSizeY;
	EngineDesc.iNumLevels = LEVEL_END;

	// 엔진 초기화
	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Fonts()))
		return E_FAIL;

	//if (FAILED(Ready_Gara()))
	//	return E_FAIL;

	// 레벨 시작
	if (FAILED(Start_Level(LEVEL_LOGO)))
		return E_FAIL;





	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;


	m_pGameInstance->Update_Engine(fTimeDelta);


#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif

	return;
}

HRESULT CMainApp::Render()
{

#ifdef _DEBUG

	//++m_iNumDraw;
	//
	//if (m_fTimeAcc >= 1.f)
	//{
	//	wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumDraw);
	//
	//	m_iNumDraw = 0;
	//	m_fTimeAcc = 0.f;
	//}	
	//
	//SetWindowText(g_hWnd, m_szFPS);
	
#endif

	if(LEVEL_GAMEPLAY !=m_iCurLevel)
		m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 0.f, 0.f));
	else
		m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));



	m_pGameInstance->Clear_DepthStencil_View();

	m_pGameInstance->Draw_Engine();

	if (FAILED(m_pGameInstance->Draw_Text(TEXT("Font_Default"), TEXT("배\n 부르다"))))
		return E_FAIL;

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eLevelID)
{
	// 어떤 레벨을 Create할 지 알아야 하기 때문에 LEVEL enum을 인자로 받아와서 호출한다.
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	/*MakeSpriteFont "넥슨lv1고딕 Bold" /FontSize:16 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 148ex.spritefont */

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Default"), TEXT("../Bin/Resources/Fonts/149ex.spritefont"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Gara()

{
	_ulong			dwByte = {};
	HANDLE			hFile = CreateFile(TEXT("../Bin/DataFiles/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoints[3] = {};
	vPoints[0] = _float3(0.f, 0.f, 10.f);
	vPoints[1] = _float3(10.f, 0.f, 0.f);
	vPoints[2] = _float3(0.f, 3.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(0.f, 0.f, 10.f);
	vPoints[1] = _float3(10.f, 0.f, 10.f);
	vPoints[2] = _float3(10.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(0.f, 0.f, 20.f);
	vPoints[1] = _float3(10.f, 0.f, 10.f);
	vPoints[2] = _float3(0.f, 0.f, 10.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(10.f, 0.f, 10.f);
	vPoints[1] = _float3(20.f, 0.f, 0.f);
	vPoints[2] = _float3(10.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);

	ID3D11Texture2D* pTexture2D = { nullptr };

	D3D11_TEXTURE2D_DESC		TextureDesc{};
	TextureDesc.Width = 128;
	TextureDesc.Height = 128;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_STAGING;
	TextureDesc.BindFlags = 0;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA		InitialData{};

	_uint* pPixel = new _uint[TextureDesc.Width * TextureDesc.Height];
	ZeroMemory(pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	//pPixel[0] = 0xff0000ff;

	InitialData.pSysMem = pPixel;
	InitialData.SysMemPitch = TextureDesc.Width * 4;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &InitialData, &pTexture2D)))
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE	MappedSubResource{};

	m_pContext->Map(pTexture2D, 0, D3D11_MAP_READ_WRITE, 0, &MappedSubResource);

	_uint* pPixels = static_cast<_uint*>(MappedSubResource.pData);

	for (size_t i = 0; i < TextureDesc.Height; i++)
	{
		for (size_t j = 0; j < TextureDesc.Width; j++)
		{
			_uint		iIndex = i * TextureDesc.Width + j;

			if (j < 64)
				pPixels[iIndex] = 0xffffffff;
			else
				pPixels[iIndex] = 0xff000000;
		}

	}

	m_pContext->Unmap(pTexture2D, 0);

	if (FAILED(SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../Bin/Resources/Textures/Terrain/Mask.dds"))))
		return E_FAIL;

	Safe_Delete_Array(pPixel);

	Safe_Release(pTexture2D);

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{	
	__super::Free();

	/*내 멤버를 정리한다. */

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);
}
