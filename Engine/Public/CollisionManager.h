#pragma once

#include "Base.h"
#include "Bounding.h"

BEGIN(Engine)

class ENGINE_DLL CCollision_Manager final : public CBase
{

private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	HRESULT											Initialize();

	HRESULT											Add_Collistionlist(const _uint iCollOption, const wstring& strColliderTag, CBounding* pInstance);

	// 충돌체 등록
	HRESULT											Regist_Update(CBounding* pBounder1, CBounding* pBounder2 = nullptr);
	HRESULT											Update_Collisions(_float fTimeDelta);
	HRESULT											Secede_Update(CBounding* pBounder1, CBounding* pBounder2 = nullptr);

	list<CBounding*>*								Find_List(TYPE eType, CBounding* pBounding);
	CBounding*										Find_Bound(TYPE eType, CBounding* pBounding);

	_bool											Check_Collision(list<CBounding*>*, list<CBounding*>* = nullptr);

private:
	_bool											Update_Impactor(_float fTimeDelta);
	_bool											Update_TargetBody(_float fTimeDelta);
	_bool											Detect_Collision(CBounding* pDest, CBounding* pSour);


private:
	void											Calculate_AABB_AABB(class CBounding_AABB* pAABB1, class CBounding_AABB* pAABB2);
	void											Calculate_AABB_OBB(class CBounding_AABB* pOBB1, class CBounding_OBB* pOBB2);
	void											Calculate_AABB_Sphere(class CBounding_AABB* pAABB1, class CBounding_Sphere* pSphere1);

	void											Calculate_0BB_0BB(class CBounding_OBB* pOBB1, class CBounding_OBB* pOBB2);
	void											Calculate_0BB_Sphere(class CBounding_OBB* pOBB1, class CBounding_Sphere* pSphere1);

	void											Calculate_Sphere_Sphere(class CBounding_Sphere* pSphere1, class CBounding_Sphere* pSphere2);

	// 밀어낼 두 충돌체를 입력
	void											Detrude_Colliders(CBounding* pDest, CBounding* pSour); 

private:
	map<const wstring, list<CBounding*>*>*			m_mapColliders[OP_END] = { nullptr };
	class CGameInstance*							m_pGameInstance = { nullptr };

	_float											m_fTotalTime = { 0.f };

public:
	static CCollision_Manager*						Create();
	virtual void									Free()				override;
};

END