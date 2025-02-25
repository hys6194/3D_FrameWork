#pragma once

#include "Base.h"

BEGIN(Engine)

class Bone final : public Base
{
private:
	Bone();
	virtual ~Bone() = default;

public:
	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_matCombinedTransform);
	}

	void Set_TransformationMatrix(_fmatrix fTransformMatrix)
	{
		XMStoreFloat4x4(&m_matTransform, fTransformMatrix);
	}

public:
	HRESULT Initialize(const aiNode* pAINode, _int iParentBoneIndex);
	void Update_CombinedTransformationMatrix(const vector<class Bone*>& Bones);
	_bool Compare_Name(const _char* pName)
	{
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

public:
	static Bone* Create(const aiNode* pAINode, _int iParentBoneIndex);
	virtual void Free() override;

};

END