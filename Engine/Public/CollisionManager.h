#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCollision_Manager final : public CBase
{
public:
	enum COLL_OPT { OP_IMPACT, OP_TARGET, OP_END };

private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	HRESULT											Initialize();

	HRESULT											Add_Collistionlist(const _uint iCollOption, const wstring& strColliderTag, class CBounding* pInstance);

	// 충돌체 등록
	HRESULT											Regist_Collision(const _uint iCollOption, const wstring& strColliderTag, class CBounding* pInstance);
	HRESULT											Secede_Update(const _uint iCollOption, const wstring& strColliderTag, class CBounding* pInstance);

	list<class CBounding*>*							Find_List(const _uint iCollOption, const wstring& strColliderTag);

	HRESULT											OnCollision_Enter();		// 업데이트에 등록할 시점 선언
	HRESULT											OnCollision_Update();		// 충돌처리 확인
	HRESULT											OnCollision_Exit();			// 업데이트에서 탈출

	// 해당 충돌체의 삭제????
	// 시발 어떻게 삭제하나요?

private:
	HRESULT											Update_Impactor();
	HRESULT											Update_TargetBody();

	
private:
	// wsrting으로 키로 객체에서 만든 충돌체들만 모아서 관리
	map<const wstring, list<CBounding*>*>*			m_mapColliders[OP_END] = { nullptr };

public:
	static CCollision_Manager*						Create();
	virtual void									Free()				override;
};

END