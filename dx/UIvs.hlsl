
struct vsin
{
    float2 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct vsout
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};


cbuffer cbuff : register(b1)
{
    float2 ws;
}

cbuffer cbuff : register(b2)
{
    float3 xf;
}
vsout main(vsin input)
{
    vsout output;
    output.pos = float4(((xf.z * input.pos + xf.xy) * 2 - ws) * ws - 1, 0, 1);
    //output.pos = mul(float4(xf.z * input.pos + xf.xy, 0, 1), prj); 
    output.tex = input.tex;
    return output;
}