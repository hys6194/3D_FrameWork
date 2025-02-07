
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_Texture;

sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
};


struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD;
    
    // 
};

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD;
};

struct PS_OUT
{
    //float4 vColor : COLOR;
    float4 vColor : SV_TARGET0;
    // 렌더타겟 뷰를 선언한 녀석이 있다, 렌더타겟은 
    //그래픽 디바이스 초기화 할 때, 백 버퍼를 생성해서 이를 통해 renderer를 그린다
    //따라서 SV_TARGET이 옳다
    // 
};


// VertexShader
// VS : 정점의 정보를 변환 + 정점의 멤버를 추가 또는 삭제한다
// VS_MAIN(float3 vPosition : POSITION, float2 vTexcoord : TEXCOORD0)
// 위 방식으로 코딩할 수는 있지만 구조체로 던지고 받는것이 좋다
// 1. 구조체 내에서 멤버를 수정할 수도 있기 때문

// 반환값을 구조체 타입으로 선언해주었는데, 이는 VS_MAIN에서 행렬을 곱해주어 
// VS의 데이터를 뷰 스페이스로 변환하여 정점연산을 마무리한다
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    
    return Out;
}


// w나누기 연산을 수행하는 함수
// w 나누기를 하면서 2차원 투영 스페이스로 변환되고, 뷰 포트로 변환한다
// 그 후, 래스터라이즈를 통해 픽셀을 생성한다.
// 래스터라이즈가 픽셀의 색을 정하는 것이라고 생각할 수 있지만, 정확하게는 픽셀을 생성하는 개념이 옳다
// 픽셀의 색을 정하는 것은 픽셀 쉐이더가 하는 일이다
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    //PS_OUT Out = { 0.f };
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    //Out.vColor = In.vTexcoord.y;
    
    
    // 코드로 구현한 알파테스트
    //if (0.5 < Out.vColor.a)
    //    discard;
    
    
    return Out;
}


technique11 DefaultTechnique
{
    pass DefaultPass0           
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile vs_5_0 PS_MAIN();
    }

    pass SpecialPass0
    {
        //VertexShader = compile vs_5_0 VS_MAIN();
        //PixelShader = compile vs_5_0 PS_MAIN();
        /* 추후에 내가 플레이어 및 몬스터가 변신한다던가 숨는다던가 
           특수한 상황에 쉐이더 기법을 사용할 때 쓰는 pass*/
        /*vs_5_0 : 쉐이더 5.0 버전임을 의미*/
    }


}
