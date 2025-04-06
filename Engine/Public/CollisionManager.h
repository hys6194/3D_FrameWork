#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCollision_Manager final : public CBase
{
public:
	enum COLL_OPT { OP_HIT, OP_PUSH, OP_END };

private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	HRESULT											Initialize();

	// 충돌체 등록
	HRESULT											Regist_Collision(const _uint iCollOption, const wstring& strColliderTag, class CBounding* pInstance);

	// 충돌 이벤트 발생한 객체 찾기
	class CBounding*								Find_Collider();

	//충돌한 객체 업데이트
	HRESULT											OnCollision_Update();

	// 해당 충돌체의 삭제????
	// 시발 어떻게 삭제하나요?

	
private:
	// wsrting으로 키로 객체에서 만든 충돌체들만 모아서 관리
	map<const wstring, list<CBounding*>*>*			m_mapColliders[OP_END];

public:
	static CCollision_Manager* Create();
	virtual void Free() override;
};

END