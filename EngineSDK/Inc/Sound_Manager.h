#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CSound_Manager : public CBase
{
private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT					Initialize();
	void					Load_SoundFile(const string& sPath = "");

public:
	void					Play_Sound(const wstring& pSoundKey, _uint iSoundIndex, float fVolume, bool bLoop = true);
	void					Play_BGM(const wstring& pSoundKey, _uint iSoundIndex, float fVolume);
	void					Stop_Sound(_uint iSoundIndex);
	void					Stop_All();
	void					Set_ChannelVolume(_uint iSoundIndex, float fVolume);

	void					Set_BGMVolume(_uint iSoundIndex, _float fVolume) { m_pChannelArr[iSoundIndex]->setVolume(fVolume); }
	void					Set_AllEffectVolume(_float fVolume)
	{
		for (int i = 1; i < MAX_PATH; ++i)
		{
			if (nullptr == m_pChannelArr[i])
				return;

			m_pChannelArr[i]->setVolume(fVolume);
		}
	}

private:
	map<const wstring, FMOD::Sound*> m_mapSound;

	//FMOD::Channel* m_pChannel;
	FMOD::Channel*			m_pChannelArr[MAX_PATH];
	FMOD::System*			m_pSystem = { nullptr };

	unsigned int			version;

	_float					m_fBGMVolume = { 0.1f };
	_float					m_fEFFVolume = { 1.0f };

private:
	wstring StrToWstr(const string& narrow_str);

public:
	static CSound_Manager*	Create();
	virtual void			Free() override;
};

END