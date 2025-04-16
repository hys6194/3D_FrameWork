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
	HRESULT Initialize(const vector<class CBone*>& _Bones, ifstream& _InStream);

	void Update_TransformationMatrix(const vector<class CBone*>& pBone, _float fCurrentTrackPosition, _uint* pKeyFrameIndex);

	_bool Save_Channel(ofstream& _OpenStream);


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
	static CChannel*	Create(const aiNodeAnim* pAIChannel, const vector<class CBone*>& pBone);
	static CChannel*	Create(const vector<class CBone*>& _Bones, ifstream& _InStream);
	virtual CChannel*	Clone() { return new CChannel(*this);	}
	virtual void		Free() override;

};

END