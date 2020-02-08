
struct VSin
{
    float4 pos : POSITION;
    float4 col : COLOR;
    float2 tex : TEXCOORD;
    uint4 jid : JOINT;
    float4 weight : WEIGHT;
};

struct VSout
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 tex : TEXCOORD;
};

cbuffer CB : register(b0)
{
    matrix prj;
};

cbuffer CB : register(b1)
{
    matrix view;
};

cbuffer CB : register(b2)
{
    matrix xf;
};

//cbuffer CB : register(b3)
//{
//    matrix bones[60];
//}

VSout main(VSin input)
{

    const matrix MVP = mul(xf, mul(view, prj));
    VSout output;
    output.col = input.col;
    output.tex = input.tex;
    output.pos = mul(input.pos, MVP);
    //float4 final =
    //   input.weight.x * mul(input.pos, bones[input.jid.x]) +
    //   input.weight.y * mul(input.pos, bones[input.jid.y]) +
    //   input.weight.z * mul(input.pos, bones[input.jid.z]) +
    //   input.weight.w * mul(input.pos, bones[input.jid.w]);
    //output.pos = mul(final, MVP);
    return output;
}