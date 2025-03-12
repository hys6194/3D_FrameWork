#pragma once

#include "VIBuffer.h"	

BEGIN(Engine)

class ENGINE_DLL CMesh : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;
public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}


public:
	virtual HRESULT Initialize_Prototype(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, MODELTYPE eType, const vector<class CBone*>& Bones);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_BoneMatrix(class CShader* pShader, const _char* pContantName, const vector<class CBone*>& Bones);

private:
	HRESULT Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	HRESULT Ready_VertexBuffer_Anim(const aiMesh* pAIMesh, const vector<CBone*>& Bones);

private:
	_char				m_szName[MAX_PATH] = {};
	_uint				m_iMaterialIndex = {};
	_uint				m_iNumBones = {};
	vector<_float4x4>	m_OffsetMatrix;
	vector<_int>		m_vecBone;
	_float4x4			m_matBone[512] = {};

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh, MODELTYPE eType, const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END