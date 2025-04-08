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

	// 충돌하는 옵션에 맞는 map컨테이너에 오브젝트의 타입을 키값으로 저장
	auto iter = m_mapColliders[iCollOption]->find(strKey);

	// 리스트가 존재하지 않는다면
	if (iter == m_mapColliders[iCollOption]->end())
	{
		auto Pair = new list<CBounding*>();

		Pair->push_back(pInstance);

		m_mapColliders[iCollOption]->emplace(strKey, Pair);
		Safe_AddRef(pInstance);
	}

	else
	{
		iter->second->push_back(pInstance);
		Safe_AddRef(pInstance);
	}

	return S_OK;
}

HRESULT CCollision_Manager::Regist_Collision(const _uint iCollOption, const wstring& strColliderTag, CBounding* pInstance)
{
	if (iCollOption >= OP_END || 
		nullptr == pInstance)
		return E_FAIL;

	size_t iKeySize = strColliderTag.find(TEXT(" "));

	// 인자로 받은 문자열의 인덱스만 제거해서 키로 사용
	wstring strKey = strColliderTag.substr(0, iKeySize);

	// 충돌하는 옵션에 맞는 map컨테이너에 오브젝트의 타입을 키값으로 저장
	auto iter = m_mapColliders[iCollOption]->find(strKey);

	// 리스트가 존재하지 않는다면
	if (iter == m_mapColliders[iCollOption]->end())
	{
		auto Pair = new list<CBounding*>();

		Pair->push_back(pInstance);

		m_mapColliders[iCollOption]->emplace(strKey, Pair);
		Safe_AddRef(pInstance);
	}

	else
	{
		iter->second->push_back(pInstance);
		Safe_AddRef(pInstance);
	}


	return S_OK;
}

HRESULT CCollision_Manager::Regist_Update(const _uint iCollOption, const wstring& strColliderTag, CBounding* pInstance)
{
	return E_NOTIMPL;
}

list<class CBounding*>* CCollision_Manager::Find_List(const _uint iCollOption, const wstring& strColliderTag)
{
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
		// 이 부분 자체가 잘못된 거 같음 Pair의 first를 확인하고 돌려야 할텐데
		// 플레이어 부터 순회
		list<CBounding*>* pList = Find_List(OP_IMPACT, TEXT("Player"));

		// 플레이어를 돌렸다면?
		if(pList == nullptr)
			pList = Find_List(OP_IMPACT, TEXT("Monster"));

		if (nullptr != pList)
		{
			for (auto& iter : *pList)
			{
				// Type이 날라가는 것이 문제임
				CBounding::BOUNDING_DESC* pDesc = reinterpret_cast<CBounding::BOUNDING_DESC*>(iter->Get_Desc());

				CCollider::TYPE test = pDesc->eType;

				int a = 1;
			}


		}
		else if (pList == nullptr)
			return E_ABORT;



		int a = 1;

		//// 해당 객체가 플레이어의 충돌원이라면
		//if (0 <= Pair.first.find(TEXT("Player")))
		//	{
		//		for (auto& iter : *Pair.second)
		//		{
		//			// 충돌 처리
		//
		//		}
		//	}
		//
		//// 몬스터들이라면
		//else
		//{
		//	for (auto& iter : *Pair.second)
		//		{
		//
		//		}
		//}
	}
	

	return S_OK;
}

HRESULT CCollision_Manager::OnCollision_Update()
{
	// 피충돌체만 호출해서 Player, Monster처리를 하는 것으로 하자

	for (auto & Pair : *m_mapColliders[OP_TARGET]);
	{

	}

	return S_OK;
}

HRESULT CCollision_Manager::OnCollision_Exit()
{
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
