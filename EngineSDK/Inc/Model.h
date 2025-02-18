#pragma once
#include "Component.h"

BEGIN (Engine)

class Model : public Component
{
private:
	Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Model(const Model& Prototype);
	virtual ~Model() = default;

public:
	HRESULT Initialize_Prototype(const _char* pFilePath);
	HRESULT Initialize(void* pArg);


public:
	static Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pFilePath);
	Component* Clone(void* pArg);
	virtual void Free() override;
};

END