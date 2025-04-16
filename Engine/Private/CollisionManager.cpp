#include "CollisionManager.h"
#include "Collider.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

#include "GameObject.h"
#include "GameInstance.h"

CCollision_Manager::CCollision_Manager()	
	: m_pGameInstance{ CGameInstance::GetInstance()}
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
	
	// 리스트가 존재하지 않는다면 리스트 생성하고 Target은 자동 등록
	if (Pair == m_mapColliders[iCollOption]->end())
	{
		// 근데 생성해버려서 insert가 맞지 않나 흐음
		auto iter = new list<CBounding*>();
	
		// 피충돌체의 경우 항상 업데이트를 돌려야 하므로 Update에 등록한다
		if (iCollOption != OP_IMPACT)
		{
			iter->push_back(pInstance);
			Safe_AddRef(pInstance);
		}

		m_mapColliders[iCollOption]->insert({ strColliderTag, iter });
	}

	// 리스트에 Target 넣기
	else
	{
		if (iCollOption != OP_IMPACT)
		{
			Pair->second->push_back(pInstance);
			Safe_AddRef(pInstance);
			m_mapColliders[iCollOption]->insert({ strColliderTag, Pair->second });
		}
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

HRESULT CCollision_Manager::Update_Collisions(_float fTimeDelta)
{
	// 충돌원 루프
	if (nullptr == m_mapColliders[OP_IMPACT] ||
		m_mapColliders[OP_IMPACT]->empty())
		return E_ABORT;

	Update_Impactor(fTimeDelta);

	Update_TargetBody(fTimeDelta);

	Update_Detector(fTimeDelta);

	return S_OK;
}

HRESULT CCollision_Manager::Secede_Update(CBounding* pBounder1, CBounding* pBounder2)
{
	if (nullptr != pBounder1)
	{
		list<CBounding*>* pList = Find_List(*pBounder1->Get_Type(), pBounder1);
		if (nullptr == pList)
			return E_ABORT;
	
		CBounding* pBouund = Find_Bound(*pBounder1->Get_Type(), pBounder1);

		// 레퍼런스 카운터 관리
		if (nullptr != pBouund)
			Safe_Release(pBounder1);
			
		pList->remove(pBounder1);
	}
	
	if (nullptr != pBounder2)
	{
		CBounding* pBouund = Find_Bound(*pBounder2->Get_Type(), pBounder2);
		if (nullptr != pBouund)
			Safe_Release(pBounder2);

		list<CBounding*>* pList = Find_List(*pBounder2->Get_Type(), pBounder2);
		if (nullptr == pList)
			return E_ABORT;

		pList->remove(pBounder2);
	}

	return S_OK;
}

list<CBounding*>* CCollision_Manager::Find_List(TYPE eType, CBounding* pBounding)
{
	// 이 함수 사용하기가 애매해 보인다
	if (eType >= TYPE_END ||
		pBounding == nullptr)
		return nullptr;
	
	CBounding::BOUNDING_INFO tInfo = *pBounding->Get_Info();

	auto Pair = m_mapColliders[tInfo.iOption]->find(tInfo.strCollTag);

	if (Pair == m_mapColliders[tInfo.iOption]->end())
		return nullptr;

	return Pair->second;
}

CBounding* CCollision_Manager::Find_Bound(TYPE eType, CBounding* pBounding)
{
	// 이 함수 사용하기가 애매해 보인다
	if (eType >= TYPE_END ||
		pBounding == nullptr)
		return nullptr;

	CBounding::BOUNDING_INFO tInfo = *pBounding->Get_Info();

	auto Pair = m_mapColliders[tInfo.iOption]->find(tInfo.strCollTag);

	if (Pair == m_mapColliders[tInfo.iOption]->end())
		return nullptr;

	for (auto& iter : *Pair->second)
	{
		if (iter == pBounding)
		{
			return iter;
		}
	}

	return nullptr;

}

_bool CCollision_Manager::Check_Collision(list<CBounding*>* pList1, list<CBounding*>* pList2, CBounding** pBound1, CBounding** pBound2)
{
	for (auto& iter1 : *pList1)
		for (auto& iter2 : *pList2)
			if (Detect_Collision(iter1, iter2))
			{
				if(pBound1 != nullptr)
					*pBound1 = iter1;

				if(pBound2 != nullptr)
					*pBound2 = iter2;

				return true;
			}

	return false;
}

_bool CCollision_Manager::Update_Impactor(_float fTimeDelta)
{
	// 피충돌체가 충돌원과 비교해서
	// 피격했는지 안했는지를 판단한다

	_bool bTest{};
	CBounding* pBounding1 = nullptr;
	CBounding* pBounding2 = nullptr;
	// 가독성 별로다 진짜
	// 아니 진짜 가독성 어떻게 하냐

	for (auto& Pair : *m_mapColliders[OP_TARGET])
	{
	
		if (Check_IncWord(Pair.first, TEXT("Player")))
		{
			// 몬스터의 충돌체
			for (auto& Pair2 : *m_mapColliders[OP_IMPACT])
			{
				if (Check_IncWord(Pair2.first, TEXT("Monster")))
				{
					if (Check_Collision(Pair.second, Pair2.second, &pBounding1, &pBounding2))
					{
						if (TYPE_SPHERE == *pBounding1->Get_Type())
							continue;

						pBounding1->Get_Collider()->Set_Coll(true);
						pBounding2->Get_Collider()->Set_Coll(true);
					}
				}
			}
		}

		// 몬스터의 몸체
		else
		{
			for (size_t i = 0; i < Pair.second->size(); i++)
			{
				for (auto& Pair2 : *m_mapColliders[OP_IMPACT])
				{
					if (Check_IncWord(Pair2.first, TEXT("Player")))
					{
						if (Check_Collision(Pair.second, Pair2.second, &pBounding1, &pBounding2))
						{
							if (TYPE_SPHERE == *pBounding1->Get_Type())
								continue;

							pBounding1->Get_Collider()->Set_Coll(true);
							pBounding2->Get_Collider()->Set_Coll(true);
						}
					}
				}
			}
		}
	}

	return bTest;
}

_bool CCollision_Manager::Update_TargetBody(_float fTimeDelta)
{
	// 피충돌체가 피충돌체와 충돌 비교하여 충돌 처리
	_bool bTest{};
	_bool bTest1{};

	// 위 로직대로 플레이어는 몬스터의 Pair에 접근해서 전부 순회해야 하고
	// 몬스터는 아래의 로직대로 돌려야함

	for (auto& Pair : *m_mapColliders[OP_TARGET])
	{
		// 플레이어의 body라면
		if (Check_IncWord(Pair.first, TEXT("Player")))
		{
			//몬스터를 순회해야 함
			for (auto& Pair2 : *m_mapColliders[OP_TARGET])
			{
				if (Check_IncWord(Pair2.first, TEXT("Monster")))
				{
					for (auto& iter1 : *Pair.second)
					{
						for (auto& iter2 : *Pair2.second)
						{
							// 충돌이 감지되면 밀어낸다
							if (Detect_Collision(iter1, iter2))
							{
								Detrude_Colliders(iter1, iter2);
							}
						}
					}

				}
			}
		}

		else if (Pair.first.find(TEXT("Monster")) != string::npos)
		{
			if (Pair.second->empty())
				continue;

			for (auto iter : *Pair.second)
			{
				for (auto iter2 : *Pair.second)
				{
					if (iter == iter2)
						continue;

					if (Detect_Collision(iter, iter2))
					{
						Detrude_Colliders(iter, iter2);
					}
				}
			}


			//for (auto iter = Pair.second->begin(); next(iter) != Pair.second->end(); ++iter)
			//{
			//	CBounding* pNextBounding = *next(iter);
			//
			//	if (*iter == nullptr || pNextBounding == nullptr)
			//		continue;
			//
			//	// 비교 처리
			//	if (Detect_Collision(*iter, pNextBounding))
			//	{
			//		Detrude_Colliders(*iter, pNextBounding);
			//	}
			//}
		}

	}

	return bTest;
}

_bool CCollision_Manager::Update_Detector(_float fTimeDelta)
{
	// 피충돌체가 피충돌체와 충돌 비교하여 충돌 처리
	_bool bTest{};
	_bool bTest1{};

	CBounding* pBounding1 = nullptr;
	CBounding* pBounding2 = nullptr;
	// 위 로직대로 플레이어는 몬스터의 Pair에 접근해서 전부 순회해야 하고
	// 몬스터는 아래의 로직대로 돌려야함

	for (auto& Pair : *m_mapColliders[OP_DETECT])
	{

		if (Check_IncWord(Pair.first, TEXT("Player")))
		{
			// 몬스터의 충돌체
			for (auto& Pair2 : *m_mapColliders[OP_DETECT])
			{
				if (Check_IncWord(Pair2.first, TEXT("Monster")))
				{
					if (Check_Collision(Pair.second, Pair2.second, &pBounding1, &pBounding2))
					{
						pBounding1->Get_Collider()->Set_Coll(true);
						pBounding2->Get_Collider()->Set_Coll(true);
					}
				}
			}
		}

		// 몬스터의 몸체
		else
		{
			for (size_t i = 0; i < Pair.second->size(); i++)
			{
				for (auto& Pair2 : *m_mapColliders[OP_DETECT])
				{
					if (Check_IncWord(Pair2.first, TEXT("Player")))
					{
						if (Check_Collision(Pair.second, Pair2.second, &pBounding1, &pBounding2))
						{
							pBounding1->Get_Collider()->Set_Coll(true);
							pBounding2->Get_Collider()->Set_Coll(true);
						}
					}
				}
			}
		}
	}

	return bTest;
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

void CCollision_Manager::Calculate_AABB_AABB(CBounding_AABB* pAABB1, CBounding_AABB* pAABB2)
{
	_float3 vSourMin, vSourMax;
	_float3 vDestMin, vDestMax;
	BoundingBox* Desc1 = pAABB1->Get_Desc();
	BoundingBox* Desc2 = pAABB2->Get_Desc();

	vSourMin = _float3(Desc1->Center.x - Desc1->Extents.x,
		Desc1->Center.y - Desc1->Extents.y,
		Desc1->Center.z - Desc1->Extents.z);
	vSourMax = _float3(Desc1->Center.x + Desc1->Extents.x,
		Desc1->Center.y + Desc1->Extents.y,
		Desc1->Center.z + Desc1->Extents.z);

	vDestMin = _float3(Desc2->Center.x - Desc2->Extents.x,
		Desc2->Center.y - Desc2->Extents.y,
		Desc2->Center.z - Desc2->Extents.z);
	vDestMax = _float3(Desc2->Center.x + Desc2->Extents.x,
		Desc2->Center.y + Desc2->Extents.y,
		Desc2->Center.z + Desc2->Extents.z);

	// 각 축에 대해 올바른 오버랩(침투 깊이) 계산
	_float fDepthX = min(vSourMax.x, vDestMax.x) - max(vSourMin.x, vDestMin.x);
	_float fDepthY = min(vSourMax.y, vDestMax.y) - max(vSourMin.y, vDestMin.y);
	_float fDepthZ = min(vSourMax.z, vDestMax.z) - max(vSourMin.z, vDestMin.z);

	// 최소 침투 깊이가 가장 작은 축을 찾습니다.
	_float fMinDepth = fDepthX;
	int iAxis = 0;

	if (fDepthY < fMinDepth)
	{
		fMinDepth = fDepthY;
		iAxis = 1;
	}
	if (fDepthZ < fMinDepth)
	{
		fMinDepth = fDepthZ;
		iAxis = 2;
	}

	// 두 객체의 중심 위치 차이를 구하여 분리시켜야 할 방향(부호)을 결정
	_float3 fCenter1 = pAABB1->Get_Desc()->Center;
	_float3 fCenter2 = pAABB2->Get_Desc()->Center;
	_float3 fCenterDiff = _float3(
		fCenter2.x - fCenter1.x,
		fCenter2.y - fCenter1.y,
		fCenter2.z - fCenter1.z);

	_float fSign = 1.f;
	switch (iAxis)
	{
		case 0: // x축
			fSign = fCenterDiff.x < 0 ? -1.f : 1.f;
			break;
		case 1: // y축
			fSign = fCenterDiff.y < 0 ? -1.f : 1.f;
			break;
		case 2: // z축
			fSign = fCenterDiff.z < 0 ? -1.f : 1.f;
			break;
	}

	_vector vAxis = XMVectorZero();
	switch (iAxis)
	{
		case 0:
			vAxis = AXIS_X;
			break;
		case 1:
			vAxis = AXIS_Y;
			break;
		case 2:
			vAxis = AXIS_Z;
			break;
	}

	// 최종 이동 오프셋 (MTV: Minimum Translation Vector)
	// 두 객체가 서로 균등하게 밀려나도록 양쪽에 절반씩 적용합니다.
	_vector vOffset = vAxis * (fMinDepth * fSign * 0.5f);

	// 두 객체의 위치를 업데이트
	_vector vPos1 = pAABB1->Get_Info()->pOwner->Get_Transform()->Get_State(CTransform::STATE_POS);
	_vector vPos2 = pAABB2->Get_Info()->pOwner->Get_Transform()->Get_State(CTransform::STATE_POS);

	pAABB1->Get_Info()->pOwner->Get_Transform()->Set_State(
		CTransform::STATE_POS, vPos1 - vOffset);
	pAABB2->Get_Info()->pOwner->Get_Transform()->Set_State(
		CTransform::STATE_POS, vPos2 + vOffset);
}

void CCollision_Manager::Calculate_AABB_OBB(CBounding_AABB* pOBB1, CBounding_OBB* pOBB2)
{
	BoundingBox AABB_Desc = *pOBB1->Get_Desc();  
	BoundingOrientedBox OBB_Desc = *pOBB2->Get_Desc();

	CBounding_OBB::OBB_DESC OBBDesc[2] = { };

	OBBDesc[0].vCenter = AABB_Desc.Center;
	OBBDesc[0].vCenterDir[0] = _float3(AABB_Desc.Extents.x, 0.f, 0.f);
	OBBDesc[0].vCenterDir[1] = _float3(0.f, AABB_Desc.Extents.y, 0.f);
	OBBDesc[0].vCenterDir[2] = _float3(0.f, 0.f, AABB_Desc.Extents.z);

	OBBDesc[0].vAlignDir[0] = _float3(1.f, 0.f, 0.f);
	OBBDesc[0].vAlignDir[1] = _float3(0.f, 1.f, 0.f);
	OBBDesc[0].vAlignDir[2] = _float3(0.f, 0.f, 1.f);

	_float3 vPoints2[8];
	pOBB2->Get_Desc()->GetCorners(vPoints2);
	OBBDesc[1].vCenter = pOBB2->Get_Desc()->Center;

	XMStoreFloat3(&OBBDesc[1].vCenterDir[0],
		(XMLoadFloat3(&vPoints2[5]) - XMLoadFloat3(&vPoints2[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc[1].vCenterDir[1],
		(XMLoadFloat3(&vPoints2[7]) - XMLoadFloat3(&vPoints2[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc[1].vCenterDir[2],
		(XMLoadFloat3(&vPoints2[0]) - XMLoadFloat3(&vPoints2[4])) * 0.5f);

	for (size_t i = 0; i < 3; i++)
	{
		_vector vDir = XMVector3Normalize(XMLoadFloat3(&OBBDesc[1].vCenterDir[i]));
		XMStoreFloat3(&OBBDesc[1].vAlignDir[i], vDir);
	}

	float fMinOverlap = FLT_MAX;
	int iOwner = 0;
	int iAxis = 0;

	
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			float fLength[3] = { 0.f, 0.f, 0.f };

			_vector vCandAxis = XMLoadFloat3(&OBBDesc[i].vAlignDir[j]);
			vCandAxis = XMVector3Normalize(vCandAxis);

			_vector vCenterDiff = XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter);
			fLength[0] = fabsf(XMVectorGetX(XMVector3Dot(vCenterDiff, vCandAxis)));

			for (int k = 0; k < 3; k++)
			{
				_vector v = XMLoadFloat3(&OBBDesc[0].vCenterDir[k]);
				fLength[1] += fabsf(XMVectorGetX(XMVector3Dot(v, vCandAxis)));
			}

			for (int k = 0; k < 3; k++)
			{
				_vector v = XMLoadFloat3(&OBBDesc[1].vCenterDir[k]);
				fLength[2] += fabsf(XMVectorGetX(XMVector3Dot(v, vCandAxis)));
			}

			float overlap = (fLength[1] + fLength[2]) - fLength[0];

			if (overlap < fMinOverlap)
			{
				fMinOverlap = overlap;
				iOwner = i;
				iAxis  = j;
			}
		}
	}

	_vector vAxis = XMVector3Normalize(XMLoadFloat3(&OBBDesc[iOwner].vAlignDir[iAxis]));

	_vector vDiff = XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter);

	_float fSign = (XMVectorGetX(XMVector3Dot(vDiff, vAxis)) < 0.f) ? -1.f : 1.f;

	_vector vMTV = XMVectorScale(vAxis, fMinOverlap * fSign);

	vMTV = XMVectorSet(XMVectorGetX(vMTV), 0.f, XMVectorGetZ(vMTV), 0.f);

	// 원래 여기서 처리하면 안되려나
	_vector vPosA = pOBB1->Get_Info()->pOwner->Get_Transform()->Get_State(CTransform::STATE_POS);
	_vector vPosB = pOBB2->Get_Info()->pOwner->Get_Transform()->Get_State(CTransform::STATE_POS);

	vPosA = XMVectorSubtract(vPosA, XMVectorScale(vMTV, 0.5f));
	vPosB = XMVectorAdd(vPosB, XMVectorScale(vMTV, 0.5f));

	pOBB1->Get_Info()->pOwner->Get_Transform()->Set_State(CTransform::STATE_POS, vPosA);
	pOBB2->Get_Info()->pOwner->Get_Transform()->Set_State(CTransform::STATE_POS, vPosB);
}

void CCollision_Manager::Calculate_AABB_Sphere(CBounding_AABB* pAABB1, CBounding_Sphere* pSphere1)
{

}

void CCollision_Manager::Calculate_0BB_0BB(CBounding_OBB* pOBB1, CBounding_OBB* pOBB2)
{
	CBounding_OBB::OBB_DESC    OBBDesc[2]{};
	
	_float fMinOverlap = FLT_MAX;
	
	_float3     vPoints1[8];
	_float3     vPoints2[8];
	
	pOBB1->Get_Desc()->GetCorners(vPoints1);
	pOBB2->Get_Desc()->GetCorners(vPoints2);
	
	OBBDesc[0].vCenter = pOBB1->Get_Desc()->Center;
	OBBDesc[1].vCenter = pOBB2->Get_Desc()->Center;
	
	// x,y,z 축에 대한 방향 벡터 -> 분리 축
	XMStoreFloat3(&OBBDesc[0].vCenterDir[0], (XMLoadFloat3(&vPoints1[5]) - XMLoadFloat3(&vPoints1[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc[0].vCenterDir[1], (XMLoadFloat3(&vPoints1[7]) - XMLoadFloat3(&vPoints1[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc[0].vCenterDir[2], (XMLoadFloat3(&vPoints1[0]) - XMLoadFloat3(&vPoints1[4])) * 0.5f);
	
	// 해당 면과 수직인 벡터 선언
	for (size_t i = 0; i < 3; i++)
		XMStoreFloat3(&OBBDesc[0].vAlignDir[i], XMVector3Normalize(XMLoadFloat3(&OBBDesc[0].vCenterDir[i])));
	
	XMStoreFloat3(&OBBDesc[1].vCenterDir[0], (XMLoadFloat3(&vPoints2[5]) - XMLoadFloat3(&vPoints2[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc[1].vCenterDir[1], (XMLoadFloat3(&vPoints2[7]) - XMLoadFloat3(&vPoints2[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc[1].vCenterDir[2], (XMLoadFloat3(&vPoints2[0]) - XMLoadFloat3(&vPoints2[4])) * 0.5f);
	
	// 해당 면과 수직인 벡터 선언
	for (size_t i = 0; i < 3; i++)
		XMStoreFloat3(&OBBDesc[1].vAlignDir[i], XMVector3Normalize(XMLoadFloat3(&OBBDesc[1].vCenterDir[i])));
	
	
	_uint iAxis1 = {};
	_uint iAxis2 = {};
	
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			_float          fLength[3] = {};
	
			// 충돌체 객체와 다른 객체의 거리와 x, y, z 방향으로 투영한 기준 벡터와 내적
			fLength[0] = fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter),
				XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]);
	
			fLength[1] = fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[0]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]) +
				fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[1]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]) +
				fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[2]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]);
	
			fLength[2] = fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[0]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]) +
				fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[1]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]) +
				fabs(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[2]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])).m128_f32[0]);
	
			_float fOverlap = fLength[1] + fLength[2] - fLength[0];
	
			if (fOverlap < fMinOverlap)
			{
				fMinOverlap = fOverlap;
				iAxis1 = i;
				iAxis2 = j;
			}
	
		}
	
	}
	
	_vector vDiffCenter = XMVectorSet(
		OBBDesc[1].vCenter.x - OBBDesc[0].vCenter.x,
		OBBDesc[1].vCenter.y - OBBDesc[0].vCenter.y,
		OBBDesc[1].vCenter.z - OBBDesc[0].vCenter.z,
		0.f);
	
	_vector vAxis = XMVectorSet(
		OBBDesc[iAxis1].vAlignDir[iAxis2].x,
		OBBDesc[iAxis1].vAlignDir[iAxis2].y,
		OBBDesc[iAxis1].vAlignDir[iAxis2].z,
		0.f);
	
	_float fDotResult = XMVectorGetX(XMVector3Dot(vAxis, vDiffCenter));
	
	_float fSign = fDotResult < 0.f ? -1.f : 1.f;
	
	_vector vMTV = XMVectorScale(XMVector3Normalize(vAxis), fSign * fMinOverlap);
	
	_vector vPos1 = XMVectorSet(
		OBBDesc[0].vCenter.x,
		OBBDesc[0].vCenter.y,
		OBBDesc[0].vCenter.z,
		0.f);
	_vector vPos2 = XMVectorSet(
		OBBDesc[1].vCenter.x,
		OBBDesc[1].vCenter.y,
		OBBDesc[1].vCenter.z,
		0.f);
	
	vPos1 = XMVectorSubtract(vPos1, XMVectorScale(vMTV, 0.5f));
	vPos2 = XMVectorAdd(vPos2, XMVectorScale(vMTV, 0.5f));
	
	vPos1 = XMVectorSetY(vPos1, pOBB1->Get_Info()->pOwner->Get_Transform()->Get_State(CTransform::STATE_POS).m128_f32[1]);
	vPos2 = XMVectorSetY(vPos2, pOBB2->Get_Info()->pOwner->Get_Transform()->Get_State(CTransform::STATE_POS).m128_f32[1]);
	
	pOBB1->Get_Info()->pOwner->Get_Transform()->Set_State(CTransform::STATE_POS, vPos1);
	pOBB2->Get_Info()->pOwner->Get_Transform()->Set_State(CTransform::STATE_POS, vPos2);
	
	//CBounding_OBB::OBB_DESC OBBDesc[2] = { };
	//
	//_float3 vPoints1[8];
	//_float3 vPoints2[8];
	//pOBB1->Get_Desc()->GetCorners(vPoints1);
	//OBBDesc[0].vCenter = pOBB1->Get_Desc()->Center;
	//
	//XMStoreFloat3(&OBBDesc[0].vCenterDir[0],
	//	(XMLoadFloat3(&vPoints1[5]) - XMLoadFloat3(&vPoints1[4])) * 0.5f);
	//XMStoreFloat3(&OBBDesc[0].vCenterDir[1],
	//	(XMLoadFloat3(&vPoints1[7]) - XMLoadFloat3(&vPoints1[4])) * 0.5f);
	//XMStoreFloat3(&OBBDesc[0].vCenterDir[2],
	//	(XMLoadFloat3(&vPoints1[0]) - XMLoadFloat3(&vPoints1[4])) * 0.5f);
	//
	//
	//pOBB2->Get_Desc()->GetCorners(vPoints2);
	//OBBDesc[1].vCenter = pOBB2->Get_Desc()->Center;
	//
	//XMStoreFloat3(&OBBDesc[1].vCenterDir[0],
	//	(XMLoadFloat3(&vPoints2[5]) - XMLoadFloat3(&vPoints2[4])) * 0.5f);
	//XMStoreFloat3(&OBBDesc[1].vCenterDir[1],
	//	(XMLoadFloat3(&vPoints2[7]) - XMLoadFloat3(&vPoints2[4])) * 0.5f);
	//XMStoreFloat3(&OBBDesc[1].vCenterDir[2],
	//	(XMLoadFloat3(&vPoints2[0]) - XMLoadFloat3(&vPoints2[4])) * 0.5f);
	//
	//
	//for (size_t i = 0; i < 3; i++)
	//{
	//	_vector vDir = XMVector3Normalize(XMLoadFloat3(&OBBDesc[1].vCenterDir[i]));
	//	XMStoreFloat3(&OBBDesc[1].vAlignDir[i], vDir);
	//}
	//
	//float fMinOverlap = FLT_MAX;
	//int iOwner = 0;
	//int iAxis = 0;
	//
	//for (size_t i = 0; i < 2; i++)
	//{
	//	for (size_t j = 0; j < 3; j++)
	//	{
	//		float fLength[3] = { 0.f, 0.f, 0.f };
	//
	//		_vector vCandAxis = XMLoadFloat3(&OBBDesc[i].vAlignDir[j]);
	//		vCandAxis = XMVector3Normalize(vCandAxis);
	//
	//		_vector vCenterDiff = XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter);
	//		fLength[0] = fabsf(XMVectorGetX(XMVector3Dot(vCenterDiff, vCandAxis)));
	//
	//		for (int k = 0; k < 3; k++)
	//		{
	//			_vector v = XMLoadFloat3(&OBBDesc[0].vCenterDir[k]);
	//			fLength[1] += fabsf(XMVectorGetX(XMVector3Dot(v, vCandAxis)));
	//		}
	//
	//		for (int k = 0; k < 3; k++)
	//		{
	//			_vector v = XMLoadFloat3(&OBBDesc[1].vCenterDir[k]);
	//			fLength[2] += fabsf(XMVectorGetX(XMVector3Dot(v, vCandAxis)));
	//		}
	//
	//		float overlap = (fLength[1] + fLength[2]) - fLength[0];
	//
	//		if (overlap < fMinOverlap)
	//		{
	//			fMinOverlap = overlap;
	//			iOwner = i;
	//			iAxis = j;
	//		}
	//	}
	//}
	//
	//_vector vAxis = XMVector3Normalize(XMLoadFloat3(&OBBDesc[iOwner].vAlignDir[iAxis]));
	//
	//_vector vDiff = XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter);
	//
	//_float fSign = (XMVectorGetX(XMVector3Dot(vDiff, vAxis)) < 0.f) ? -1.f : 1.f;
	//
	//_vector vMTV = XMVectorScale(vAxis, fMinOverlap * fSign);
	//
	//vMTV = XMVectorSet(XMVectorGetX(vMTV), 0.f, XMVectorGetZ(vMTV), 0.f);
	//
	//// 원래 여기서 처리하면 안되려나
	//_vector vPosA = pOBB1->Get_Info()->pOwner->Get_Transform()->Get_State(CTransform::STATE_POS);
	//_vector vPosB = pOBB2->Get_Info()->pOwner->Get_Transform()->Get_State(CTransform::STATE_POS);
	//
	//vPosA = XMVectorSubtract(vPosA, XMVectorScale(vMTV, 0.5f));
	//vPosB = XMVectorAdd(vPosB, XMVectorScale(vMTV, 0.5f));
	//
	//pOBB1->Get_Info()->pOwner->Get_Transform()->Set_State(CTransform::STATE_POS, vPosA);
	//pOBB2->Get_Info()->pOwner->Get_Transform()->Set_State(CTransform::STATE_POS, vPosB);
}								

void CCollision_Manager::Calculate_0BB_Sphere(CBounding_OBB* pOBB1, CBounding_Sphere* pSphere1)
{

}

void CCollision_Manager::Calculate_Sphere_Sphere(CBounding_Sphere* pSphere1, CBounding_Sphere* pSphere2)
{

}

void CCollision_Manager::Detrude_Colliders(CBounding* pDest, CBounding* pSour)
{
	// 서로 밀어내야함
	pDest->Get_Type();
	switch (*pDest->Get_Type())
	{
		case TYPE_AABB:
		{
			if (TYPE_AABB == *pSour->Get_Type())
				Calculate_AABB_AABB(static_cast<CBounding_AABB*>(pDest), static_cast<CBounding_AABB*>(pSour));

			else if (TYPE_OBB == *pSour->Get_Type())
				Calculate_AABB_OBB(static_cast<CBounding_AABB*>(pDest), static_cast<CBounding_OBB*>(pSour));

			else
				Calculate_AABB_Sphere(static_cast<CBounding_AABB*>(pDest), static_cast<CBounding_Sphere*>(pSour));

			break;
		}
		case TYPE_OBB:
		{
			if (TYPE_AABB == *pSour->Get_Type())
				Calculate_AABB_OBB(static_cast<CBounding_AABB*>(pSour), static_cast<CBounding_OBB*>(pDest));

			else if (TYPE_OBB == *pSour->Get_Type())
				Calculate_0BB_0BB(static_cast<CBounding_OBB*>(pDest), static_cast<CBounding_OBB*>(pSour));

			else
				Calculate_0BB_Sphere(static_cast<CBounding_OBB*>(pDest), static_cast<CBounding_Sphere*>(pSour));

			break;
		}
		case TYPE_SPHERE:
		{
			if (TYPE_AABB == *pSour->Get_Type())
				Calculate_AABB_Sphere(static_cast<CBounding_AABB*>(pSour), static_cast<CBounding_Sphere*>(pDest));

			else if (TYPE_OBB == *pSour->Get_Type())
				Calculate_0BB_Sphere(static_cast<CBounding_OBB*>(pSour), static_cast<CBounding_Sphere*>(pDest));

			else
				Calculate_Sphere_Sphere(static_cast<CBounding_Sphere*>(pDest), static_cast<CBounding_Sphere*>(pSour));
			break;
		}
	
	default:
		break;
	}

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
