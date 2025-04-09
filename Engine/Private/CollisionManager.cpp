#include "CollisionManager.h"
#include "Bounding.h"

CCollision_Manager::CCollision_Manager()
{
}

HRESULT CCollision_Manager::Initialize()
{
	// 옵션 만큼 생성
	for (size_t i = 0; i < OP_END; i++)
	{
		m_mapColliders[i] = new map< const wstring, list<CBounding*>*>();
	}

	return S_OK;
}

HRESULT CCollision_Manager::Add_Collistionlist(const _uint iCollOption, const wstring& strColliderTag, CBounding* pInstance)
{
	if (iCollOption >= OP_END ||
		nullptr == pInstance)
		return E_FAIL;

	size_t iKeySize = strColliderTag.find(TEXT(" "));

	// 인자로 받은 문자열의 인덱스만 제거해서 키로 사용
	wstring strKey = strColliderTag.substr(0, iKeySize);

	// 충돌하는 타입에 맞는 map컨테이너에 오브젝트의 타입을 키값으로 저장
	auto iter = m_mapColliders[iCollOption]->find(strKey);

	// 리스트가 존재하지 않는다면 리스트 생성
	if (iter == m_mapColliders[iCollOption]->end())
	{
		auto Pair = new list<CBounding*>();

		m_mapColliders[iCollOption]->emplace(strKey, Pair);
	}

	// 최종적으로 몬스터의 부위별로 충돌체를 list에 받아온 것

	return S_OK;
}

HRESULT CCollision_Manager::Regist_Update(CComponent* pCollCom1, CComponent* pCollCom2)
{
	// 인자로 받아온 콜라이더를 Update 리스트에 추가하여 돌게한다
	CBounding* pBounding1{ nullptr }; CBounding* pBounding2{ nullptr };
	if (nullptr != pCollCom1)
		pBounding1 = dynamic_cast<CCollider*>(pCollCom1)->Get_Collider();

	if (nullptr != pCollCom2)
		pBounding2 = dynamic_cast<CCollider*>(pCollCom2)->Get_Collider();

	// 이런 식으로 해당 맵 컨테이너에 키값이 Player인 list를 들고오는데 이때 Pair를 가져오게 하는 것이 좋아보이긴 함
	list<CBounding*>* pList = Find_List(OP_IMPACT, TEXT("Monster"));

	return S_OK;
}


HRESULT CCollision_Manager::Secede_Update(CComponent* pCollCom1, CComponent* pCollCom2)
{
	// 인자로 받아온 콜라이더를 Update 리스트에 제외한다

	return S_OK;
}

list<class CBounding*>* CCollision_Manager::Find_List(const _uint iCollOption, const wstring& strColliderTag)
{
	// 이 함수 사용하기가 애매해 보인다
	if (iCollOption >= OP_END)
		return nullptr;

	// 충돌하는 옵션에 맞는 map컨테이너에 오브젝트의 타입을 키값으로 저장
	for (auto& pair : *m_mapColliders[iCollOption])
	{
		if (pair.first.find(strColliderTag) != std::string::npos)
		{
			return pair.second;
		}
	}

	return nullptr;
}

HRESULT CCollision_Manager::OnCollision_Enter()
{
	// 충돌원 루프
	for (auto& Pair : *m_mapColliders[OP_IMPACT])
	{
		if (Pair.first.find(TEXT("Player")))
		{

		}
		else
		{

		}

	}



	
	return S_OK;
}

HRESULT CCollision_Manager::OnCollision_Update()
{
	// 피충돌체만 호출해서 Player, Monster처리를 하는 것으로 하자

	for (auto & Pair : *m_mapColliders[OP_TARGET]);
	{

	}

	list<CBounding*>* pList = Find_List(OP_IMPACT, TEXT("Monster"));


	return S_OK;
}

HRESULT CCollision_Manager::OnCollision_Exit()
{
	return S_OK;
}

HRESULT CCollision_Manager::Update_Impactor()
{
	// 피충돌체가 충돌원과 비교해서
	// 피격했는지 안했는지를 판단한다

	return S_OK;
}

HRESULT CCollision_Manager::Update_TargetBody()
{
	// 피충돌체가 피충돌체와 충돌 비교하여 

	return S_OK;
}

CCollision_Manager* CCollision_Manager::Create()
{
	CCollision_Manager* pInstance = new CCollision_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Collision_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollision_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < OP_END; i++)
	{
		for (auto& Pair : *m_mapColliders[i])
		{
			for (auto iter = Pair.second->begin();
				iter != Pair.second->end();
				iter++)
			{
				if (nullptr == *iter)
					return;

				Safe_Release(*iter);
			}

			Pair.second->clear();

			Safe_Delete(Pair.second);
		}

		m_mapColliders[i]->clear();

		Safe_Delete(m_mapColliders[i]);
	}


}
