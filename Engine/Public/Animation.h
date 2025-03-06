#pragma once

#include "Base.h"

BEGIN(Engine)

class Animation final : public Base
{
private:
	Animation();
	virtual ~Animation() = default;

public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const vector<class Bone*>& pBone, vector<_uint>& iCurrentKeyFrameIndices);

	// 애니메이션의 행렬 업데이트
	_bool Update_TransformationMatrix(const vector<class Bone*>& pBone, _float fTimeDelta, _bool bIsLoop, _float* pCurrentTrackPosition, vector<_uint>& vecKeyFrameIndex, _float* pPreTrackPosition = nullptr, vector<_uint>* vecPreAnimKeyFrameIndex = nullptr);
	void KeyFrame_Reset();
private:
	// 애니메이션의 이름
	_char						m_szName[MAX_PATH] = {};

	// 애니메이션의 총 길이
	// 시간의 개념이 아님 fTime * fTrickPerSecond하여 누적한 결과 값임
	_float						m_fDuration = {};

	// 애니메이션의 재생 속도
	_float						m_fTrickPerSecond = {};

	// Channel = 애니메이션의 특정 키 프레임에서 가지는 뼈의 정보
	_uint						m_iNumChannel = {};
	vector<class Channel*>		m_vecChannel = {};

	_bool						m_bIsAnimEnd = { false };



public:
	static Animation* Create(const aiAnimation* pAIAnimation, const vector<class Bone*>& pBone, vector<_uint>& iCurrentKeyFrameIndices);
	virtual void Free() override;
};

END

