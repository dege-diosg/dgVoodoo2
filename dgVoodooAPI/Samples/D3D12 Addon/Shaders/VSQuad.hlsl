// *****************************************************************************
// File:			VSQuad.hlsl
//
// Description:		Vertex shader used for drawing the textured quad
//
// Contact person:	DG
//
// *****************************************************************************

// --- Input signature ---------------------------------------------------------

struct INPUT
{
	float2	pos			: POSITION0;
	float2	srcTexUV	: TEXCOORD0;
};

// --- Output signature --------------------------------------------------------

struct OUTPUT
{
	float4	pos			: SV_POSITION;
	float2	srcTexUV	: TEXCOORD0;
};

// --- Resources ---------------------------------------------------------------

// --- Shader code -------------------------------------------------------------

OUTPUT	main (INPUT i)
{
	OUTPUT o;
	o.pos		= float4 (i.pos, 0.0f, 1.0f);
	o.srcTexUV	= i.srcTexUV;

	return o;
}
