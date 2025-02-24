#pragma once
#include "Component.h"

BEGIN (Engine)

class ENGINE_DLL Model : public Component
{
private:
	Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Model(const Model& Prototype);
	virtual ~Model() = default;

public:
	virtual HRESULT Initialize_Prototype(MODELTYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render(_uint iMeshIndex);

public:
	_uint Get_NumMeshes() const 
	{
		return m_iNumMeshes;
	}

public:
	HRESULT Bind_Material(class Shader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iMeshIndex, _uint iTextureIndex);

private:
	const aiScene*						m_pAIScene = { nullptr };
	Assimp::Importer					m_Importer;
	MODELTYPE							m_eModelType = {};
	_float4x4							m_PreTransformMatrix = {};

	_uint								m_iNumMeshes = {};
	vector<class Mesh*>					m_vecMesh;

	_uint								m_iNumMaterials = {};
	vector<class MeshMaterial*>		m_vecMaterial;

private:
	HRESULT Ready_Meshes();	
	HRESULT Ready_Materials(const _char* pModelFilePath);


public:
	static Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	Component* Clone(void* pArg);
	virtual void Free() override;
};

END