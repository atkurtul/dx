struct PSin
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 tex : TEXCOORD;
};

Texture2D txt : register(t0);
SamplerState sample;

float4 main(PSin input) : SV_TARGET
{
    return txt.Sample(sample, input.tex);
}