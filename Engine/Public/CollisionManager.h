#pragma once

#include "Base.h"

BEGIN(Engine)

class CCollision_Manager : public CBase
{
private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	// 충돌체 등록
	HRESULT		 Regist_Collision();

	// 충돌 이벤트 발생한 객체 찾기
	class CBounding* Find_Collider();

	//충돌한 객체 업데이트
	HRESULT		 OnCollision_Update();

	// 해당 충돌체의 삭제????
	// 시발 어떻게 삭제하나요?

	
//private:
//	map<>

public:
	static CCollision_Manager* Create();
	virtual void Free() override;
};

END