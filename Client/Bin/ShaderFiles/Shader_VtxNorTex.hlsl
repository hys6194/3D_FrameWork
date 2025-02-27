// ºû ¿¬»êÀ» Ã³¸®ÇÏ´Â hlsl ÆÄÀÏ
// ÃßÈÄ¿¡ pass¸¦ ´Ã·Á¼­ ºû¿¬»ê Ã³¸® ¹æ½ÄÀ» ´Þ¸®ÇÑ´Ù

matrix      g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

//// ºûÀÇ ¹æÇâ
//float4      g_vLightDir = float4(1.f, -1.f, 1.f, 0.f);
//
//// ºû »ö
//float4      g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f);
//
//float4      g_vLightAmbient = float4(1.f, 1.f, 1.f, 1.f);
//float4      g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f);

// ºûÀÇ ¹æÇâ
float4      g_vLightDir;

// ºû »ö
float4      g_vLightDiffuse;
float4      g_vLightAmbient;
float4      g_vLightSpecular;


texture2D   g_DiffuseTexture;
float4      g_vMtrlAmbient = float4(0.3f, 0.3f, 0.3f, 1.f);
float4      g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);

float4      g_vCamPosition;


sampler DefaultSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    // Æþ ½¦ÀÌµù
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    
    
    return Out;
}

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    // ±×¸²ÀÚ ¾øÀÌ Ãâ·Â
    //Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord * 30.f);
    
    // Æþ ¼ÎÀÌµù
    // vector vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord * 30.f);
    // 
    // float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);
    // 
    // Out.vColor = g_vLightDiffuse * vDiffuse * fShade;
    
    vector vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord * 30.f);
    
    // 0º¸´Ù ÀÛÀ¸¸é 0 return, 1 º¸´Ù Å©¸é 1 return
    float fShade = saturate(dot(normalize(g_vLightDir) * -1.f, In.vNormal));
    
    vector vLook = In.vWorldPos - g_vCamPosition;
    vector vReflect = reflect(normalize(g_vLightDir), In.vNormal);
    
    float fSpecular = pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 50.f);
    
    Out.vColor = g_vLightDiffuse * vDiffuse * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
        + (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    return Out;
}

technique11 DefaultTechnique
{
    pass DefaultPass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}



