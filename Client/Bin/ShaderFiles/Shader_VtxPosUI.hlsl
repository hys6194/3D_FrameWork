// 지오메트릭 셰이더

#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;

vector g_vCamPosition;

struct VS_IN
{
    float3 vPosition : POSITION;
    
    // 행 기준으로 행렬이 만들어 지는 것을 의미
    row_major float4x4 TransformMatrix : WORLD;
    float2 vLifeTime : TEXCOORD0;
};


struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vPSize    : PSIZE;
    float2 vLifeTime : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    /* 받아온 정점정보를 가지고 필요한 연산을 수행해 나간다 */
    VS_OUT Out = (VS_OUT) 0;
  
    // 정점의 로컬 위치를 TransformMatrix을 곱하여 로컬 행렬에 곱하고 월드에 곱하면서 월드로 변환
    vector vPosition = mul(vector(In.vPosition, 1.f), In.TransformMatrix);
    
    Out.vPosition = mul(vPosition, g_WorldMatrix);
    
    // 로컬 행렬에서 스케일을 가져와서 저장
    Out.vPSize = float3(length(In.TransformMatrix._11_12_13), length(In.TransformMatrix._21_22_23), length(In.TransformMatrix._31_32_33));
    Out.vLifeTime = In.vLifeTime;
  
    return Out;
}

struct GS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vPSize    : PSIZE;
    float2 vLifeTime : TEXCOORD0;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD5;
};

 // 행렬 정보를 던질 때, inout TriangleStream<GS_OUT>으로 던져야 함
[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
    GS_OUT Out[4];
    
    // 카메라를 기준으로 하는 빌보드 평면의 두 축을 계산한다
    float4 vLook = g_vCamPosition - In[0].vPosition;
    float4 vRight = normalize(float4(cross(float3(0.f, 1.f, 0.f), vLook.xyz), 0.f)) * In[0].vPSize.x * 0.5f;
    
    float4 vUp = normalize(float4(cross(vLook.xyz, vRight.xyz), 0.f)) * In[0].vPSize.y * 0.5f;
    
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
    
    // 월드에 적용된 버퍼를 투영 스페이스까지 올리는 작업
    Out[0].vPosition = mul(In[0].vPosition + vRight + vUp, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    
    Out[1].vPosition = mul(In[0].vPosition - vRight + vUp, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;
    
    Out[2].vPosition = mul(In[0].vPosition - vRight - vUp, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;
    
    Out[3].vPosition = mul(In[0].vPosition + vRight - vUp, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;
    
    
    // 그릴 때 stripe 형태로 그리게 되어서 인덱스 그리는 순서가 0,1,2 0,2,3 순서가 아닌,
    // 0,1,2, 1,2,0 순서로 그리게 되어 그리는 순서를 조정해줘야 한다
    OutStream.Append(Out[0]);
    OutStream.Append(Out[1]);
    OutStream.Append(Out[2]);
    OutStream.RestartStrip();
    
    OutStream.Append(Out[0]);
    OutStream.Append(Out[2]);
    OutStream.Append(Out[3]);
    OutStream.RestartStrip();
    
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD5;
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
    
    if(0.3f >= Out.vColor.a)
        discard;
    
    Out.vColor.a = In.vLifeTime.x - In.vLifeTime.y;
 
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
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);


        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}



