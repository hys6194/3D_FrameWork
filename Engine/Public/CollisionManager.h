#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCollision_Manager final : public CBase
{

private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	HRESULT											Initialize();

	HRESULT											Add_Collistionlist(const _uint iCollOption, const wstring& strColliderTag, class CBounding* pInstance);

	// 충돌체 등록
	HRESULT											Regist_Update(class CBounding* pBounder1, class CBounding* pBounder2 = nullptr);
	HRESULT											Update_Collisions();
	HRESULT											Secede_Update(class CBounding* pBounder1, class CBounding* pBounder2 = nullptr);

	list<class CBounding*>*							Find_List(TYPE eType, class CBounding* pBounding);

	HRESULT											OnCollision_Enter();		// 업데이트에 등록할 시점 선언
	HRESULT											OnCollision_Update();		// 충돌처리 확인
	HRESULT											OnCollision_Exit();			// 업데이트에서 탈출



private:
	_bool											Update_Impactor();
	_bool											Update_TargetBody();
	_bool											Detect_Collision(CBounding* pDest, CBounding* pSour);

	
private:
	
	map<const wstring, list<CBounding*>*>*			m_mapColliders[OP_END] = { nullptr };

public:
	static CCollision_Manager*						Create();
	virtual void									Free()				override;
};

END