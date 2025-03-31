
#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_vColor;

struct VS_IN
{
    float3 vPosition : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
};


VS_OUT VS_MAIN(VS_IN In)
{
   
    VS_OUT Out = (VS_OUT)0;
    

    matrix matWV, matWVP;    
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);    
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP); 
    
    return Out;
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};



PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_vColor;    
    
    return Out;
}

PS_OUT PS_MAIN1(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = float4(0.f, 1.f, 0.f, 0.f);
    
    return Out;
}
 


/* 하드웨어 장치의 지원여부에 따라 다른 버젼의 셰이더를 빌드할 수 있도록 추가적으로 테크니커를 만들수 있다.*/ 
technique11 DefaultTechnique
{
    /* 내가 적용하고 싶은 셰이더 기법들의 캡슐화 */
    /* 이 패스에서 사용할 여러가지 셰이더의 진입점 함수를 지정한다. */

    /* 일반적으로 렌더링한다 : 빛연산 + 스펙큘러 + 림라이트 + 그림자 + ssao */ 
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass DefaultPass1
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN1();
    }


}



