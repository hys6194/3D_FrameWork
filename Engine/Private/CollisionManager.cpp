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
		iter->second->push_back(pInstance);


	return S_OK;
}

CBounding* CCollision_Manager::Find_Collider()
{


	return nullptr;
}

HRESULT CCollision_Manager::OnCollision_Update()
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
