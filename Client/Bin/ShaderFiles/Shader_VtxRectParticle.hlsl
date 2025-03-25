
#include "Engine_Shader_Defines.hlsli"

/* hlsl에서의 행렬타입 */
// float2x2, float3x3, float4x4 == matrix, float1x4

/* hlsl에서의 벡터타입 */
// float2, float3, float4 == vector 

/* 상수집합 == 컨스턴트테이블 */ 
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    
    float4 vRight : TEXCOORD1;
    float4 vUp : TEXCOORD2;
    float4 vLook : TEXCOORD3;
    float4 vTranslation : TEXCOORD4;
};


struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    /* 받아온 정점정보를 가지고 필요한 연산을 수행해 나간다 */
    VS_OUT Out = (VS_OUT) 0;
  
  
    matrix LocalTransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    vector vPosition = mul(vector(In.vPosition, 1.f), LocalTransformMatrix);
  
    /* 로컬위치벡터 * 월드행렬 * 뷰행렬* 투영행렬 */      
    matrix matWV, matWVP;
  
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
  
    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexcoord = In.vTexcoord;
  
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};


/* Pixel Shader : 이렇게 생성된 픽셀을 픽셀쉐이더로 던져서 픽셀의 최종적인 색을 결정할 수 있도록 한다.*/ 
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if(0.4f >= Out.vColor.a)
        discard;
    
    return Out;
}
 


/* 하드웨어 장치의 지원여부에 따라 다른 버젼의 셰이더를 빌드할 수 있도록 추가적으로 테크니커를 만들수 있다.*/ 
technique11 DefaultTechnique
{
    /* 내가 적용하고 싶은 셰이더 기법들의 캡슐화 */
    /* 이 패스에서 사용할 여러가지 셰이더의 진입점 함수를 지정한다. */

    /* 일반적으로 렌더링한다 : 빛연산 + 스펙큘러 + 림라이트 + 그림자 + ssao */ 
    pass DefaultPass0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}



