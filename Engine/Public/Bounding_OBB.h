#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_OBB final : public CBounding
{
public:
	typedef struct tagBoudingOBBDesc
	{
		class CGameObject* pOwner;

		TYPE		eType;
		_wstring	strCollTag;
		_uint		iOption;
		_float3		vCenter;
		_float3		vExtents;
		_float3		vRotation;
		_uint		iAttack; // 아...이거 절대 아닌데
	}BOUNDING_OBB_DESC;

	typedef struct tagOBBDesc
	{
		_float3		vCenter;
		_float3		vCenterDir[3]; /*파란색*/
		_float3		vAlignDir[3];
	}OBB_DESC;

private:
	CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_OBB() = default;

public:
	BoundingOrientedBox*				Get_Desc() {
		return m_pDesc;
	}

public:
	HRESULT								Initialize(const CBounding_OBB::BOUNDING_OBB_DESC* pDesc, class CCollider* pOwner);
	virtual void						Update(_fmatrix WorldMatrix) override;
public:									
	virtual _bool						Intersect(TYPE eType, CBounding* pTargetBound) override;
										
#ifdef _DEBUG							
public:									
	virtual HRESULT						Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif									
										
private:								
	BoundingOrientedBox*				m_pLocalDesc = { nullptr };
	BoundingOrientedBox*				m_pDesc = { nullptr };

private:								
	_bool								Intersect_OBB(CBounding_OBB* pTargetBound);
	OBB_DESC							Compute_OBBDesc();
										
										
public:									
	static CBounding_OBB*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding_OBB::BOUNDING_OBB_DESC* pDesc, class CCollider* pOwner);
	virtual void						Free() override;

};

END