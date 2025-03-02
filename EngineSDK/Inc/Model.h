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

	const _float4x4* Get_BoneMatrix(const _char* pBoneName);

public:
	void Set_AnimationIndex(_uint iAnimationIndex, _bool isLoop = false);

public:
	virtual HRESULT Initialize_Prototype(MODELTYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);

public:
	_bool	Play_Animation(_float fTimeDelta);
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
	_int								m_iCurrentAnimationIndex = { -1 };
	_uint								m_iNumAnimations = {};
	vector<class Animation*>			m_Animations;

	// 애니메이션의 현재 재생위치
	vector<_float>						m_vecCurrentTrackPosition = {};

	// 채널에서의 현재 키프레임의 정보 위치
	vector<vector<_uint>>				m_vecKeyFrameIndex;


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