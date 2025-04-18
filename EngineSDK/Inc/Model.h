#pragma once

#include "Component.h"
#include "Animation.h"
#include "Bone.h"


BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	_uint								Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	vector<class CMesh*>				Get_Meshes()
	{
		return m_vecMesh;
	}

	const _uint							Get_PreAnimIndex() { return m_iPreAnimationIndex; };

	const _float4x4*					Get_BoneMatrix(const _char* pBoneName);

	const _bool							Get_Interpolate() {
		return m_bIsInter;
	}

	const vector<_float4>				Get_DeltaVector()
	{
		return m_vecBone[2]->Get_DeltaVector();
	}

	const _float4						Get_Delta()
	{
		return m_vecBone[2]->Get_Delta();
	}

	const void							Reset_Delta()
	{
		m_vecBone[2]->Reset_Delta();
	}

	const _float						Get_CurAnimationDuration()
	{
		return m_Animations[m_iCurrentAnimationIndex]->Get_Duration();
	}

	const _float						Get_CurAnimationTrackPosition()
	{
		return m_vecCurrentTrackPosition[m_iCurrentAnimationIndex];
	}

	const _uint							Get_CurKeyFrameIndex()
	{
		return m_iCurKeyFrameIndex;
	}

	_bool			Get_AnimEnd()
	{
		return m_Animations[m_iCurrentAnimationIndex]->Get_AnimEnd();
	}

public:
	//재생하려고 하는 애니메이션, 루프, 보간
	void								Set_AnimationIndex(_uint iAnimationIndex, _bool isLoop = false, _bool IsInter = true);
	void								Set_Interpolate(_bool bIsInter);

public:
	void								Set_PreAnimation(_uint iPreAnimationIndex);

public:
	void								Interpolate_Animation(_float fRatio = 0.1f);

public:
	virtual HRESULT						Initialize_Prototype(MODELTYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT						Initialize_Prototype(MODELTYPE _eType, const _char* _pModelFilePath, const _char* _pBinaryFilePath, _fmatrix _PreTransformMatrix);
	virtual HRESULT						Initialize_Prototype(const _char* _pBinaryFilePath, _fmatrix _PreTransformMatrix);
	virtual HRESULT						Initialize(void* pArg) override;
	virtual HRESULT						Render(_uint iMeshIndex);

public:
	static CModel*						Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, MODELTYPE _eType, const _char* _pModelFilePath, const _char* _pBinaryFilePath, _fmatrix _PreTransformMatrix = XMMatrixIdentity());
	static CModel*						Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const _char* _pBinaryFilePath, _fmatrix _PreTransformMatrix = XMMatrixIdentity());

public:
	_bool								Play_Animation(_float fTimeDelta, CGameObject* pObject = nullptr);

public:
	void								Reset_PreAnimation();

public:
	HRESULT								Bind_Material(class CShader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iMeshIndex, _uint iTextureIndex);
	HRESULT								Bind_BoneMatrix(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);

public:
	_bool								CheckRayColl_Mesh(_vector vPos, _vector vDir, _float* _fDistance, _float4* _fCoord, _vector vScale, _vector vRotation, _vector vTranslation);
	_bool								DotPoint_InMesh(_vector vPos, _vector vDir, _float* _fDistance, _float4* _fCoord, _vector vScale, _vector vRotation, _vector vTranslation);

private:
	const aiScene*						m_pAIScene = { nullptr };
	Assimp::Importer					m_Importer;
	MODELTYPE							m_eModelType = {};
	_float4x4							m_PreTransformMatrix = {};

	_uint								m_iNumMeshes = { };
	vector<class CMesh*>				m_vecMesh;

	_uint								m_iNumMaterials = {};
	vector<class CMeshMaterial*>		m_vecMaterial;

	vector<class CBone*>				m_vecBone;

	_bool								m_bIsLoop = { false };
	_bool								m_bIsInter = { false };
	_int								m_iCurrentAnimationIndex = { -1 };
	_uint								m_iNumAnimations = {};
	vector<class CAnimation*>			m_Animations;

	// 애니메이션의 현재 재생위치
	vector<_float>						m_vecCurrentTrackPosition = {};

	// 채널에서의 현재 키프레임의 정보 위치
	vector<vector<_uint>>				m_vecKeyFrameIndex;

	_uint								m_iPreAnimationIndex = {};

	_uint								m_iPreKeyFrameIndex = {};
	_uint								m_iCurKeyFrameIndex = {};

	_float								m_fPreTrackPos = {};
	_float								m_fCurTrackPos = {};
	_float								m_fRatio = { 0.f };

	_uint								m_iNumBone = {};

	vector<class CChannel*>				m_pPreChannel;
	vector<class CChannel*>				m_pCurChannel;

	vector<class CChannel*>				m_vecChannel;

	KEYFRAME							m_pPreKeyFrame = { };
	KEYFRAME							m_pCurKeyFrame = { };

private:
	HRESULT								Ready_Bones_Load(_uint _iNumBones, ifstream& _InStream);
	HRESULT								Ready_Meshes_Save(ofstream& _OutStream);
	HRESULT								Ready_Materials_Save(const _char* _pModelFilePath, ofstream& _OutStream);
	HRESULT								Ready_Meshes_Load(ifstream& _InStream);
	HRESULT								Ready_Materials_Load(ifstream& _InStream);
	HRESULT								Ready_Animation_Load(ifstream& _InStream);

private:
	HRESULT								Ready_Bones(const aiNode* pAINode, _int iParentBoneIndex);
	HRESULT								Ready_Meshes();
	HRESULT								Ready_Materials(const _char* pModelFilePath);
	HRESULT								Ready_Animations();

public:
	static CModel*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual CComponent*					Clone(void* pArg) override;
	virtual void						Free();
};

END