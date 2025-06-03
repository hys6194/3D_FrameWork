#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	//CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const vector<class CBone*>& pBone, vector<_uint>& iCurrentKeyFrameIndices);
	HRESULT Initialize(const vector<class CBone*>& _Bones, ifstream& _InStream, vector<_uint>& _CurrentKeyFrameIndices);

	// 애니메이션의 행렬 업데이트
	_bool Update_TransformationMatrix(const vector<class CBone*>& pBone, _float fTimeDelta, _bool bIsLoop, _float* pCurrentTrackPosition, vector<_uint>& vecKeyFrameIndex);
	void KeyFrame_Reset();

public:
	_bool Save_Animation(ofstream& _OpenStream);


public:
	vector<class CChannel*> Get_Channel()
	{
		return m_vecChannel;
	}

public:
	const _float Get_Duration()
	{
		return m_fDuration;
	}

	const _bool Get_AnimEnd()
	{
		return m_bIsAnimEnd;
	}

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
	vector<class CChannel*>		m_vecChannel = {};

	_bool						m_bIsAnimEnd = { false };


public:
	static CAnimation*			Create(const aiAnimation* pAIAnimation, const vector<class CBone*>& pBone, vector<_uint>& iCurrentKeyFrameIndices);
	static CAnimation*			Create(const vector<class CBone*>& _Bones, ifstream& _InStream, vector<_uint>& _CurrentKeyFrameIndices);
	virtual CAnimation*			Clone()				{ return new CAnimation(*this); };
	virtual void				Free() override;
};

END

