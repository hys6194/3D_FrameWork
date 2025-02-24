#pragma once

#include "VIBuffer.h"	

BEGIN(Engine)
class Mesh : public VIBuffer
{
private:
	Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Mesh(const Mesh& Prototype);
	virtual ~Mesh() = default;

public:
	virtual HRESULT Initialize_Prototype(const aiMesh* pAIMesh);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;
};

END