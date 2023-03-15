// *****************************************************************************
// File:			PSGlass.hlsl
//
// Description:		Pixel shader used for drawing the glassified quad
//
// Contact person:	DG
//
// *****************************************************************************

// --- Input signature ---------------------------------------------------------

struct INPUT
{
	float4	pos			: SV_POSITION;
	float2	srcTexUV	: TEXCOORD0;
};

// --- Output signature --------------------------------------------------------

struct OUTPUT
{
	float4	color0		: SV_TARGET0;
};

// --- Resources ---------------------------------------------------------------

SamplerState		samplerPt	: register (s0);
SamplerState		samplerBi	: register (s1);
Texture2D<float4>	textures[2] : register (t3, space1);

cbuffer				RootPSConstBuffer : register (b0, space2)
{
	float3			sepiaColor;
	float			colorIntensity;
}

// --- Shader code -------------------------------------------------------------

OUTPUT	main (INPUT i)
{
	const float3 bwFactors	= float3 (0.3125f, 0.5f, 0.1875f);
	OUTPUT o;

	float4 t1			= textures[1].Sample (samplerBi, i.srcTexUV);
	float4 t0			= textures[0].Sample (samplerPt, i.srcTexUV);

	t0.rgb				= lerp (dot (t0.rgb, bwFactors), t0.rgb, colorIntensity) * sepiaColor;

	o.color0			= lerp (t0, t1, t1.a);

	return o;
}
