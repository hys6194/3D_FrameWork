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
	virtual HRESULT Initialize_Prototype(const _char* pFilePath, MODELTYPE eType);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render();

private:
	const aiScene*				m_pAIScene = { nullptr };
	Assimp::Importer			m_Importer;
	MODELTYPE					m_eModelType = {};

	_uint						m_iNumMeshes = {};
	vector<class Mesh*>			m_vecMesh;

private:
	HRESULT Ready_Meshes();

public:
	static Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const _char* pFilePath);
	Component* Clone(void* pArg);
	virtual void Free() override;
};

END