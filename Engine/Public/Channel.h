#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel : public CBase
{
private:
	CChannel(); 
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(const aiNodeAnim* pAIChannel, const vector<class CBone*>& pBone);
	void Update_TransformationMatrix(const vector<class CBone*>& pBone, _float fCurrentTrackPosition, _uint* pKeyFrameIndex);


public:
	vector<KEYFRAME> Get_KeyFrame()
	{
		return m_vecFrame;
	}


private:
	_char				m_szName[MAX_PATH] = {};

	// 특정 뼈의 현재 정보위치를 저장
	_uint				m_iBoneIndex = {};
	_uint				m_iNumFrameKeys = {};

	vector<KEYFRAME>	m_vecFrame = {};
	
public:
	static CChannel* Create(const aiNodeAnim* pAIChannel, const vector<class CBone*>& pBone);
	virtual void Free() override;

};

END