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
	HRESULT Initialize_Prototype(const _char* pFilePath);
	HRESULT Initialize(void* pArg);

private:
	const aiScene*				m_pAIScene = { nullptr };
	Assimp::Importer			m_Importer;


	vector<class Mesh*>			m_vecMesh;

public:
	static Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pFilePath);
	Component* Clone(void* pArg);
	virtual void Free() override;
};

END