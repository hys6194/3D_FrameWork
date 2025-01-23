#pragma once

#include "Base.h"

// 원형객체를 만드는 클래스
// GameObject나 Component들을 레벨별로 구분해서 모아둘 클래스이기도 하다
// 원형객체를 만들되 내보낼 때 사본객체를 내보낼 예저우

BEGIN(Engine)

class Prototype : public Base
{
private:
	Prototype(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Prototype() = default;

public:
	// 레벨별로 Prototype을 만들 함수
	HRESULT Initialize(_uint iNumLevel);
	HRESULT Add_Prototype(const _wstring& szPrototypeName, _uint iLevelIndex, Base* pPrototype);
	Base* Clone_Prototype(PROTOTYPE ePrototype, const _wstring& szPrototypeName, _uint iLevelIndex, void* pArg);
	void Clear(_uint iLevelIndex);

private:
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };
	_uint								m_iNumLevel = {};

private:
	// 원형객체들을 레벨별로 나누어서 보관하기 위해 맵으로 저장
	// 키값으로 레벨의 이름을 가져와서 찾고 꺼내기 위함
	map<const _wstring, Base*>*		    m_pPrototype = { nullptr };
	typedef map<const _wstring, Base*>		PROTOTYPES;

private:
	// 베이스 포인터의 이유 = 베이스 상속받잖아요;
	Base* Find_Prototype(const _wstring& szPrototypeName, _uint iLevelIndex);

public:
	static Prototype* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumLevel);
	virtual void Free() override;

};

END

