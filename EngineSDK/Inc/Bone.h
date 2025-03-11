#pragma once

#include "Base.h"

/*1. aiNode : 전체 뼈의 상속구조를 표현한다 + 뼈의 상태 행렬의 정보를 가진다. */
/*2. aiBone : 특정 메시에 소속. 이 본이 특정메시를 구성하는 정점들 중, 어떤 정점에게 영향을 준다 + 얼마나 영향을 준다. 
		+ OffsetMatrix정보까지 전달해준다. */
/*2_1. OffsetMatrix : 이 뼈를 메시에게 적용하기위해서 얼마만큼의 추가적인 변환을 거쳤어야하는가?! */
/*2_2. 특정상황에 한 뼈 셋트를 서로 다른 형태의 메시에게 적용해야할 필요가 생긴다. (아저씨, 어린이, 할아버지, 할머니, 아가씨) */

/*3. aiNodeAnim :  */

BEGIN(Engine)

class CBone final : public CBase
{
	
private:
	CBone();
	virtual ~CBone() = default;

public:
	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_matCombinedTransform);
	}

	const _float4x4* Get_CombinedTransformfloat4x4ptr() const
	{
		return &m_matCombinedTransform;
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix)
	{
		XMStoreFloat4x4(&m_matTransform, TransformationMatrix);
	}

public:
	_float Get_DeltaPower() 
	{
		return m_fCurDelta;
	}

public:
	void Reset_Delta() 
	{
		m_vPreDelta = { 0.f,0.f,0.f,1.f }; 
		m_fCurDelta = { 0.f };
	}

public:
	HRESULT Initialize(const aiNode* pAINode, _int iParentBoneIndex);
	void Update_CombinedTransformationMatrix(const vector<class CBone*>& Bones, const _float4x4* pPreTransformMatrix);
	void Update_Combine_RootMatrix(const vector<class CBone*>& Bones, const _float4x4* pPreTransformMatrix, class CGameObject* pObject = nullptr);
	_bool Compare_Name(const _char* pName) {
		return !strcmp(m_szName, pName);
	}
	
private:
	// 뼈의 이름
	_char					m_szName[MAX_PATH] = {};

	// 특정 메쉬에 영향을 주는 뼈의 행렬
	_float4x4				m_matTransform = {};

	// 위 행렬의 부모 뼈의 행렬
	// 부모의 부모의 부모의 부모의 .... 부모 형식으로 이어나가게 됨
	_float4x4				m_matCombinedTransform = {};

	// 부모뼈의 인덱스
	_int					m_iParentBoneIndex = {};

	_float4					m_vPreDelta = { 0.f,0.f,0.f,1.f };
	_float					m_fCurDelta = {};


public:
	static CBone* Create(const aiNode* pAINode, _int iParentBoneIndex);
	virtual CBone* Clone();
	virtual void Free() override;
};

END