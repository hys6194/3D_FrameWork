#include "CollisionManager.h"
#include "Bounding.h"

CCollision_Manager::CCollision_Manager()
{
}

HRESULT CCollision_Manager::Regist_Collision(const wstring& strColliderTag)
{



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
    return new CCollision_Manager();
}

void CCollision_Manager::Free()
{
	__super::Free();
}
