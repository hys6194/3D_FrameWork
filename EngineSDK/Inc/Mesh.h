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

	// 근데 이거 안쓸 것 같은데 차라리 Mesh에서 함수를 만들고 모델에서 보내는 것도 방법일듯 함
	vector<VTXMESH>*    Get_Vertices()		{ return &m_vecVertices; }
	vector<VTXANIMESH>* Get_VerticesAnim()  { return &m_vecVerticesAnim; }


public:
	virtual HRESULT						Initialize_Prototype(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, MODELTYPE eType, const vector<class CBone*>& Bones);
	virtual HRESULT						Initialize_Prototype(const aiMesh* _pAIMesh, MODELTYPE _eModelType, const vector<class CBone*>& _Bones, _fmatrix _PreTransformMatrix, ofstream& _OutStream);
	virtual HRESULT						Initialize_Prototype(MODELTYPE _eModelType, const vector<class CBone*>& _Bones, _fmatrix _PreTransformMatrix, ifstream& _InStream);
	virtual HRESULT						Initialize(void* pArg) override;

public:
	HRESULT								Bind_BoneMatrix(class CShader* pShader, const _char* pContantName, const vector<class CBone*>& Bones);

public:
	_bool								Search_Picked_Face(_vector vPos, _vector vDir, _float* _fDistance, _float4* _fCoord, _vector vScale, _vector vRotation, _vector vTranslation);

private:
	HRESULT								Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	HRESULT								Ready_VertexBuffer_ForNonAnim_Save(const aiMesh* _pAIMesh, _fmatrix PreTransformMatrix, ofstream& _OutStream);
	HRESULT								Ready_VertexBuffer_ForNonAnim_Load(_fmatrix PreTransformMatrix, ifstream& _InStream);

	HRESULT								Ready_VertexBuffer_Anim(const aiMesh* pAIMesh, const vector<CBone*>& Bones);
	HRESULT								Ready_VertexBuffer_ForAnim_Save(const aiMesh* _pAIMesh, const vector<class CBone*>& _Bones, ofstream& _OutStream);
	HRESULT								Ready_VertexBuffer_ForAnim_Load(const vector<class CBone*>& _Bones, ifstream& _InStream);

private:
	_char								m_szName[MAX_PATH] = {};
	_bool								m_bColl = {};
	_uint								m_iMaterialIndex = {};
	_uint								m_iNumBones = {};
	_float								m_fPreDistance = {};
	vector<_float4x4>					m_OffsetMatrix;
	vector<_int>						m_vecBone;
	_float4x4							m_matBone[512] = {};

	vector<VTXMESH>						m_vecVertices;
	vector<VTXANIMESH>					m_vecVerticesAnim;

	vector<_uint>						m_vecIndicesIndex;

public:
	static CMesh*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh, MODELTYPE eType, const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix);
	static CMesh*						Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const aiMesh* _pAIMesh, MODELTYPE _eModelType, const vector<class CBone*>& _Bones, _fmatrix _PreTransformMatrix, ofstream& _OutStream);
	static CMesh*						Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, MODELTYPE _eModelType, const vector<class CBone*>& _Bones, _fmatrix _PreTransformMatrix, ifstream& _InStream);
	virtual CComponent*					Clone(void* pArg) override;
	virtual void						Free() override;
};

END