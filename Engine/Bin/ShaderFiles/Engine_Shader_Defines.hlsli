
// 레스터 라이즈의 기본 세팅
RasterizerState RS_Default
{
    FillMode = SOLID;
    // 뒷면 컬링 설정
    CullMode = BACK;
    // 객체의 그려지는 방향 세팅
    // false 일 때 ,시계 방향
    FrontCounterClockwise = false;
};
// 레스터 라이징의 윤곽선 렌더링 방식
RasterizerState RS_Wireframe
{
    FillMode = WIREFRAME;
    CullMode = BACK;
    FrontCounterClockwise = false;
};

// 컬링 없는 옵션
RasterizerState RS_Cull_None
{
    FillMode = SOLID;
    CullMode = FRONT;
    FrontCounterClockwise = false;
};

DepthStencilState DSS_Default
{
    // 깊이 테스트 활성화
    DepthEnable = true;
    // 깊이 버퍼의 모든 값을 기록
    DepthWriteMask = ALL;
    // 작거나 같으면 렌더링
    DepthFunc = LESS_EQUAL;
};

DepthStencilState DSS_None
{
    DepthEnable = false;
    // 값을 쓰지 않음
    DepthWriteMask = ZERO;    
};


BlendState BS_Default
{
    BlendEnable[0] = false;
};

BlendState BS_AlphaBlend
{
    BlendEnable[0] = true;
    //BlendEnable[1] = true;

    //SrcBlend = SRC_ALPHA;
    //DestBlend= INV_SRC_ALPHA;
    //BlendOp = ADD;


    SrcBlend[0] = SRC_ALPHA;
    DestBlend[0] = INV_SRC_ALPHA;
    BlendOp[0] = ADD;
};
