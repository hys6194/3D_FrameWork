#include "CollisionManager.h"
#include "Collider.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CCollision_Manager::CCollision_Manager()
{
}

HRESULT CCollision_Manager::Initialize()
{
	for (size_t i = 0; i < OP_END; i++)
	{
		m_mapColliders[i] = new map< const wstring, list<CBounding*>*>();
	}
	
	return S_OK;
}

HRESULT CCollision_Manager::Add_Collistionlist(const _uint iCollOption, const wstring& strColliderTag, CBounding* pInstance)
{
	auto Pair = m_mapColliders[iCollOption]->find(strColliderTag);
	
	// 리스트가 존재하지 않는다면 리스트 생성
	if (Pair == m_mapColliders[iCollOption]->end())
	{
		auto iter = new list<CBounding*>();
	
		// 피충돌체의 경우 항상 업데이트를 돌려야 하므로 Update에 등록한다
		if (iCollOption == OP_TARGET)
		{
			iter->push_back(pInstance);
			Safe_AddRef(pInstance);
		}

		m_mapColliders[iCollOption]->emplace(strColliderTag, iter);
	}

	return S_OK;
}

HRESULT CCollision_Manager::Regist_Update(CBounding* pBounder1, CBounding* pBounder2)
{
	
	if (nullptr != pBounder1)
	{
		list<CBounding*>* pList = Find_List(*pBounder1->Get_Type(), pBounder1);
		if (nullptr == pList)
			return E_FAIL;

		pList->push_back(pBounder1);
		Safe_AddRef(pBounder1);

	}
	
	if (nullptr != pBounder2)
	{
	
		list<CBounding*>* pList = Find_List(*pBounder2->Get_Type(), pBounder2);
		if (nullptr == pList)
			return E_FAIL;
	
		pList->push_back(pBounder2);
		Safe_AddRef(pBounder2);

	}
	
	return S_OK;
}

HRESULT CCollision_Manager::Update_Collisions()
{
	// 충돌원 루프
	if (nullptr == m_mapColliders[OP_IMPACT] ||
		m_mapColliders[OP_IMPACT]->empty())
		return E_ABORT;

	_bool bTest = Update_Impactor();

	if (bTest == true) return S_OK;

	return S_OK;
}

HRESULT CCollision_Manager::Secede_Update(CBounding* pBounder1, CBounding* pBounder2)
{
	if (nullptr != pBounder1)
	{
	
		list<CBounding*>* pList = Find_List(*pBounder1->Get_Type(), pBounder1);
		if (nullptr == pList)
			return E_ABORT;
	
		pList->remove(pBounder1);
		Safe_Release(pBounder1);
	}
	
	if (nullptr != pBounder2)
	{
		list<CBounding*>* pList = Find_List(*pBounder2->Get_Type(), pBounder2);
		if (nullptr == pList)
			return E_ABORT;
	
		pList->remove(pBounder2);aaaaa
		Safe_Release(pBounder2);
	}

	return S_OK;
}


list<CBounding*>* CCollision_Manager::Find_List(TYPE eType, CBounding* pBounding)
{
	// 이 함수 사용하기가 애매해 보인다
	if (eType >= TYPE_END ||
		pBounding == nullptr)
		return nullptr;

	switch (eType)
	{
		case TYPE_SPHERE:
		{
			CBounding_Sphere::BOUNDING_SPHERE_INFO tInfo = *static_cast<CBounding_Sphere*>(pBounding)->Get_Info();

			auto iter = m_mapColliders[tInfo.iOption]->find(tInfo.strCollTag);

			if (iter == m_mapColliders[tInfo.iOption]->end())
				return nullptr;

			return iter->second;
		}

		case TYPE_AABB:
		{
			CBounding_AABB::BOUNDING_AABB_INFO tInfo = *static_cast<CBounding_AABB*>(pBounding)->Get_Info();

			auto iter = m_mapColliders[tInfo.iOption]->find(tInfo.strCollTag);

			if (iter == m_mapColliders[tInfo.iOption]->end())
				return nullptr;

			return iter->second;
		}

		case TYPE_OBB:
		{
			CBounding_OBB::BOUNDING_OBB_INFO tInfo = *static_cast<CBounding_OBB*>(pBounding)->Get_Info();

			auto iter = m_mapColliders[tInfo.iOption]->find(tInfo.strCollTag);

			if (iter == m_mapColliders[tInfo.iOption]->end())
				return nullptr;

			return iter->second;
		}

		default:
			break;
	}

	return nullptr;
}

HRESULT CCollision_Manager::OnCollision_Enter()
{

	
	return S_OK;
}

HRESULT CCollision_Manager::OnCollision_Update()
{
	// 피충돌체만 호출해서 Player, Monster처리를 하는 것으로 하자



	return S_OK;
}

HRESULT CCollision_Manager::OnCollision_Exit()
{
	return S_OK;
}

_bool CCollision_Manager::Update_Impactor()
{
	// 피충돌체가 충돌원과 비교해서
	// 피격했는지 안했는지를 판단한다
	_bool bTest{};

	for (auto& Pair : *m_mapColliders[OP_TARGET])
	{
		// 플레이어의 몸체
		// 사실상 몸체는 하나뿐이라 하나만 돌림

		if (Pair.first.find(TEXT("Player")) != string::npos)
		{
			// 몬스터의 충돌체
			for (auto& Pair2 : *m_mapColliders[OP_IMPACT])
			{
				if (Pair2.first.find(TEXT("Monster")) != string::npos)
				{
					for (auto& iter : *Pair.second)
					{
						for (auto& iter2 : *Pair2.second)
						{
							bTest = Detect_Collision(iter, iter2);

							if (bTest == true)
							{
								iter->Get_Collider()->Set_Coll(bTest);
								iter2->Get_Collider()->Set_Coll(bTest);

								//Secede_Update(iter, iter2);
							}
						}
					}
				}
			}
		}

		// 몬스터의 몸체
		else
		{
			for (size_t i = 0; i < Pair.second->size(); i++)
			{

			}
		}
	}

	return bTest;
}

_bool CCollision_Manager::Update_TargetBody()
{
	// 피충돌체가 피충돌체와 충돌 비교하여 

	return S_OK;
}

_bool CCollision_Manager::Detect_Collision(CBounding* pDest, CBounding* pSour)
{
	_bool bIsColl = false;

	pDest->Get_Type();
	switch (*pDest->Get_Type())
	{
		case TYPE_AABB:
		{
			if (TYPE_AABB == *pSour->Get_Type())
				bIsColl = static_cast<CBounding_AABB*>(pDest)->Get_Desc()->Intersects(*static_cast<CBounding_AABB*>(pSour)->Get_Desc());

			else if (TYPE_OBB == *pSour->Get_Type())
				bIsColl = static_cast<CBounding_AABB*>(pDest)->Get_Desc()->Intersects(*static_cast<CBounding_OBB*>(pSour)->Get_Desc());

			else
				bIsColl = static_cast<CBounding_AABB*>(pDest)->Get_Desc()->Intersects(*static_cast<CBounding_Sphere*>(pSour)->Get_Desc());

			break;
		}
		case TYPE_OBB:
		{
			if (TYPE_AABB == *pSour->Get_Type())
				bIsColl = static_cast<CBounding_OBB*>(pDest)->Get_Desc()->Intersects(*static_cast<CBounding_AABB*>(pSour)->Get_Desc());

			else if (TYPE_OBB == *pSour->Get_Type())
				bIsColl = static_cast<CBounding_OBB*>(pDest)->Get_Desc()->Intersects(*static_cast<CBounding_OBB*>(pSour)->Get_Desc());

			else
				bIsColl = static_cast<CBounding_OBB*>(pDest)->Get_Desc()->Intersects(*static_cast<CBounding_Sphere*>(pSour)->Get_Desc());

			break;
		}

		case TYPE_SPHERE:
		{
			if (TYPE_AABB == *pSour->Get_Type())
				bIsColl = static_cast<CBounding_Sphere*>(pDest)->Get_Desc()->Intersects(*static_cast<CBounding_AABB*>(pSour)->Get_Desc());

			else if (TYPE_OBB == *pSour->Get_Type())
				bIsColl = static_cast<CBounding_Sphere*>(pDest)->Get_Desc()->Intersects(*static_cast<CBounding_OBB*>(pSour)->Get_Desc());

			else
				bIsColl = static_cast<CBounding_Sphere*>(pDest)->Get_Desc()->Intersects(*static_cast<CBounding_Sphere*>(pSour)->Get_Desc());

			break;
		}
		
		default:
			break;
	}

	return bIsColl;
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
				if (nullptr == (*iter))
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
