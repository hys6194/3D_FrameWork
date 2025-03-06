#pragma once

#include "Base.h"

BEGIN(Engine)

class Channel : public Base
{
private:
	Channel(); 
	virtual ~Channel() = default;

public:
	HRESULT Initialize(const aiNodeAnim* pAIChannel, const vector<class Bone*>& pBone);
	void Update_TransformationMatrix(const vector<class Bone*>& pBone, _float fCurrentTrackPosition, _uint* pKeyFrameIndex);


public:
	KEYFRAME Get_KeyFrame(_uint iKeyFrameIndex)
	{
		return m_vecFrame[iKeyFrameIndex];
	}

	_bool Get_IsAnim()
	{
		return m_vecFrame.back().bIsAnim;
	}

private:
	_char				m_szName[MAX_PATH] = {};
	// 특정 뼈의 현재 정보위치를 저장
	_uint				m_iBoneIndex = {};
	_uint				m_iAnimBoneIndex = {};
	_uint				m_iNumFrameKeys = {};

	vector<KEYFRAME>	m_vecFrame = {};
	vector<KEYFRAME>	m_vecAnimFrame = {};

	_bool				m_bIsmatched;
	
public:
	static Channel* Create(const aiNodeAnim* pAIChannel, const vector<class Bone*>& pBone);
	virtual void Free() override;

};

END