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
	static Channel* Create(const aiNodeAnim* pAIChannel, const vector<class Bone*>& pBone);
	virtual void Free() override;

};

END