#include "Navi_Cell.h"
#include "GameInstance.h"
// 일단 피킹하고 삼각형을 그리는 흐름을 생각해보자
// Navi 클릭하고 메쉬에 제대로 피킹이 되었다면 이 클래스를 생성하여
// 삼각형을 미리 그리고 Cell의 행렬 정보를 완벽하게 만들었다면
// Render를 끄는 방식으로 만들자
//
//아니 진짜 이걸 클론해야하는게 맞냐 진짜로????
//그러면 base로 상속받고 GameObject를 따로 관리하면서 Transform을 선언하던가 해야할 듯한데?
//근데 Transform이 필요한가?
//애초에 삼각형만 그려내는 기능과 위치를 그에 맞게 그려내야하는 거 아닌가
//그러면 필요 없어 보이는데 Base로 바꾸자

CNavi_Cell::CNavi_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
    , m_pContext { pContext}
    , m_pGameInstance {CGameInstance::GetInstance()}
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CNavi_Cell::Initialize()
{
    return S_OK;
}S

void CNavi_Cell::Update(_float fTimeDelta)
{
}

HRESULT CNavi_Cell::Render()
{
	return S_OK;
}

HRESULT CNavi_Cell::Bind_SR()
{
	return S_OK;
}

HRESULT CNavi_Cell::Ready_Component()
{
	return S_OK;
}

CNavi_Cell* CNavi_Cell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNavi_Cell* pInstance = new CNavi_Cell(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed To Created : Navi_Cell");
        Safe_Release(pInstance);
    }

    return pInstance;
}

//CGameObject* CNavi_Cell::Clone(void* pArg)
//{
//    CNavi_Cell* pInstance = new CNavi_Cell(*this);
//
//
//    // GameObject라서 좀 불합리한데 귀찮다 그냥하자
//    if (FAILED(pInstance->Initialize(pArg)))
//    {
//        MSG_BOX("Failed To Cloned : Navi_Cell");
//    }
//        Safe_Release(pInstance);
//
//    return pInstance;
//} 

void CNavi_Cell::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);
}
