#pragma once

// 외부로 보여줄 만한 컴포넌트들의 헤더를 모아둔다
#include "FSM.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Collider.h"
#include "Navigation.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Flat_Terrain.h"
#include "VIBuffer_Rect_Instancing.h"

#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

BEGIN(Engine)

class CPrototype_Manager final : public CBase
{
private:
	CPrototype_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPrototype_Manager() = default;

public:
	HRESULT							Initialize(_uint iNumLevels);
	// 어떤 레벨, 프로토타입 이름, 프로토타입의 주소를 받아와 원본객체 생성
	HRESULT							Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CBase* pPrototype);

	// 사본 객체 생성하고 map 컨테이너에 대입
	CBase*							Clone_Prototype(PROTOTYPE ePrototype, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	
	// 어떤 레벨의 원본객체를 삭제할 지
	void							Clear(_uint iLevelIndex);

	HRESULT							Collect_PrototypeTag(_uint iLevelIndex);

	vector<wstring>*				Get_PrototypeTag(_uint iLevelIndex)
	{
		return &m_vecProtoTag;
	}

private:
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };
	_uint							m_iNumLevels = {};

private:

	// 원형객체들을 레벨별로 나누어서 보관하기위해
	map<const wstring, CBase*>*		m_pPrototype = { nullptr };
	typedef map<const wstring, CBase*> PROTOTYPES;

	// EngineDesc에 VECTOR 타입을 넣지 않았는데 왜?
	// memcpy할 때 컴파일러가 터짐
	// 
	// Clone 함수는 언제?
	// -> Prototype을 가진 Object를 만들 때 만들 예정

	vector<wstring>					m_vecProtoTag;

private:
	CBase*							Find_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag);

public:
	static CPrototype_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumLevels);
	virtual void Free() override;
};

END