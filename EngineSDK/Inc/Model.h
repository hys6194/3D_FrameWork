#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL Model final : public Component
{
private:
	Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Model(const Model& Prototype);
	virtual ~Model() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	const _uint Get_PreAnimIndex() { return m_iPreAnimationIndex; };

	const _float4x4* Get_BoneMatrix(const _char* pBoneName);

	const _bool Get_Interpolate() {
		return m_bIsInter;
	}

public:
	// 대부분의 애니메이션은 보간이 필요하므로 마지막 인자의 기본값 = true
	void Set_AnimationIndex(_uint iAnimationIndex, _bool isLoop = false, _bool IsInter = true);
	void Set_Interpolate(_bool bIsInter) { m_bIsInter = bIsInter; };

public:
	void Set_PreAnimation(_uint iPreAnimationIndex);

public:
	void Interpolate_Animation(_float fRatio = 0.f);

public:
	virtual HRESULT Initialize_Prototype(MODELTYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);

public:
	_bool	Play_Animation(_float fTimeDelta);

public:
	void	Reset_PreAnimation();


public:
	HRESULT Bind_Material(class Shader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iMeshIndex, _uint iTextureIndex);
	HRESULT Bind_BoneMatrix(class Shader* pShader, const _char* pConstantName, _uint iMeshIndex);


private:
	const aiScene*						m_pAIScene = { nullptr };
	Assimp::Importer					m_Importer;
	MODELTYPE							m_eModelType = {};
	_float4x4							m_PreTransformMatrix = {};

	_uint								m_iNumMeshes = { };
	vector<class Mesh*>					m_vecMesh;

	_uint								m_iNumMaterials = {};
	vector<class MeshMaterial*>			m_vecMaterial;

	vector<class Bone*>					m_vecBone;

	_bool								m_bIsLoop = { false };
	_bool								m_bIsInter = { true };
	_int								m_iCurrentAnimationIndex = { -1 };
	_uint								m_iNumAnimations = {};
	vector<class Animation*>			m_Animations;

	// 애니메이션의 현재 재생위치
	vector<_float>						m_vecCurrentTrackPosition = {};

	// 채널에서의 현재 키프레임의 정보 위치
	vector<vector<_uint>>				m_vecKeyFrameIndex;

	_uint								m_iPreAnimationIndex = { 0 };

	_uint								m_iPreKeyFrameIndex = { 0 };
	_uint								m_iCurKeyFrameIndex = { 0 };

	_float								m_fPreTrackPos = { 0 };
	_float								m_fInterTrackPos = { 0 };
	_float								m_fRatio = { 0.f };

	_uint								m_iNumBone = { 0 };

	vector<class Channel*>				m_pPreChannel;
	vector<class Channel*>				m_pCurChannel;

	KEYFRAME							m_pPreKeyFrame = { };
	KEYFRAME							m_pInterKeyFrame = { };

private:
	HRESULT Ready_Bones(const aiNode* pAINode, _int iParentBoneIndex);
	HRESULT Ready_Meshes();
	HRESULT Ready_Materials(const _char* pModelFilePath);
	HRESULT Ready_Animations();

public:
	static Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual Component* Clone(void* pArg) override;
	virtual void Free();
};

END