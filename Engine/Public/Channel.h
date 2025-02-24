#pragma once

#include "Base.h"

BEGIN(Engine)

class Channel : public CBase
{
private:
	Channel(); 
	virtual ~Channel() = default;

public:
	HRESULT Initialize(const aiNodeAnim* pAIChannel, const vector<class CBone*>& pBone);
	void Update_TransformationMatrices(const vector<class CBone*>& pBone, _float fCurrentTrackPosition);

private:
	_char				m_szName[MAX_PATH] = {};
	// 특정 뼈의 현재 정보위치를 저장
	_uint				m_iBoneIndex = {};
	_uint				m_iNumFrameKeys = {};

	// 현재 키프레임의 정보 위치
	_uint				m_iCurrentKeyFrameIndex = {};
	vector<KEYFRAME>	m_vecFrame = {};

public:
	static Channel* Create(const aiNodeAnim* pAIChannel, const vector<class CBone*>& pBone);
	virtual void Free() override;

};

END