

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 position3D : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{
    float4	textureColor;
    float3	lightDir;
    float	lightIntensity;
    float4	color;

    textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
}

