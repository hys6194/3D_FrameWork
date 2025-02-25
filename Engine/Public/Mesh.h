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
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}


public:
	virtual HRESULT Initialize_Prototype(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, MODELTYPE eType);
	virtual HRESULT Initialize(void* pArg) override;

private:
	HRESULT Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	HRESULT Ready_VertexBuffer_Anim(const aiMesh* pAIMesh);

private:
	_uint				m_iMaterialIndex = {};
	_uint				m_iNumBones = {};

public:
	static Mesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh, MODELTYPE eType, _fmatrix PreTransformMatrix);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;
};

END