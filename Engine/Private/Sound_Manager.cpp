#include "Sound_Manager.h"

namespace fs = std::filesystem;

CSound_Manager::CSound_Manager()
{
}

HRESULT CSound_Manager::Initialize()
{
	FMOD::System_Create(&m_pSystem);

	m_pSystem->getVersion(&version);

	m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);

    return S_OK;
}

HRESULT CSound_Manager::Load_SoundFile(const string& sPath)
{
	for (const auto& entry : fs::directory_iterator(sPath))
	{
		if (!entry.is_regular_file() || entry.path().extension() != ".ogg")
			continue;

		FMOD::Sound* pSound = nullptr;

		string sPath = entry.path().string();
		FMOD_RESULT eRes = m_pSystem->createSound(sPath.c_str(), FMOD_DEFAULT, 0, &pSound);
		if (eRes != FMOD_OK)
			continue;

		string sName = entry.path().filename().stem().string();
		wstring wsKey = StrToWstr(sName);

		m_mapSound.insert({ wsKey, pSound });
	}

	return S_OK;
}

void CSound_Manager::Play_Sound(const wstring& pSoundKey, _uint iSoundIndex, float fVolume, bool bLoop)
{
	auto iter = m_mapSound.find(pSoundKey);
	if (iter == m_mapSound.end())
		return;

	bool bPlay = FALSE;

	if (m_pChannelArr[iSoundIndex]->isPlaying(&bPlay))
	{
		m_pSystem->playSound(iter->second, 0, FALSE, &m_pChannelArr[iSoundIndex]);
	}
	else if (!bLoop)
	{
		// bLoop가 false 라면 해당 채널에서 재생이되고 있어도 끊고 바로 소리를 재생 
		m_pChannelArr[iSoundIndex]->stop();
		m_pSystem->playSound(iter->second, 0, FALSE, &m_pChannelArr[iSoundIndex]);
	}
	m_pChannelArr[iSoundIndex]->setVolume(m_fEFFVolume * fVolume);
	m_pSystem->update();
}

void CSound_Manager::Play_BGM(const wstring& pSoundKey, _uint iSoundIndex, float fVolume)
{
	auto iter = m_mapSound.find(pSoundKey);
	if (iter == m_mapSound.end())
		return;

	if (m_pChannelArr[iSoundIndex])
	{
		m_pChannelArr[iSoundIndex]->stop();
		m_pChannelArr[iSoundIndex] = nullptr;
	}

	m_pSystem->playSound(iter->second, 0, FALSE, &m_pChannelArr[iSoundIndex]);
	m_pChannelArr[iSoundIndex]->setMode(FMOD_LOOP_NORMAL);
	m_pChannelArr[iSoundIndex]->setVolume(m_fBGMVolume * fVolume);
	m_pSystem->update();
}

void CSound_Manager::Stop_Sound(_uint iSoundIndex)
{
	m_pChannelArr[iSoundIndex]->stop();
}

void CSound_Manager::Stop_All()
{
	for (int i = 0; i < MAX_PATH; ++i)
		m_pChannelArr[i]->stop();
}

void CSound_Manager::Set_ChannelVolume(_uint iSoundIndex, float fVolume)
{
	m_pChannelArr[iSoundIndex]->setVolume(fVolume);
	m_pSystem->update();
}


wstring CSound_Manager::StrToWstr(const string& narrow_str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &narrow_str[0], (int)narrow_str.size(), NULL, 0);
	wstring wstr(size_needed, 0);

	MultiByteToWideChar(CP_UTF8, 0, &narrow_str[0], (int)narrow_str.size(), &wstr[0], size_needed);

	return wstr;
}

CSound_Manager* CSound_Manager::Create()
{
	CSound_Manager* pInstance = new CSound_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Level_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSound_Manager::Free()
{
	__super::Free();

	for (auto& Mypair : m_mapSound)
		Mypair.second->release();

	m_mapSound.clear();
	m_pSystem->release();
}
