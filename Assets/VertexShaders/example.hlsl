/*
	This is an example of a vertex shader
*/

// Uniforms
//=========

uniform float g_secondsElapsed = 1.0;

uniform float4x4 g_modelToWorldTransform;
uniform float4x4 g_worldToViewTransform;
uniform float4x4 g_viewToProjectionTransform;

// Shader program
//===============

void main(  in const float3 i_position : POSITION, 
			in const float3 i_color : COLOR0, 
			in const float3 i_normal : NORMAL,
			in const float3 i_tangent : TANGENT,
			in const float3 i_binormal : BINORMAL,
			in const float2 i_texcoord : TEXCOORD0,
			out float4 o_position : POSITION, 
			out float4 o_color : COLOR0,
			out float3 o_world_position : TEXCOORD1,
			out float3 o_normal : NORMAL0,
			out float3 o_tangent : TANGENT,
			out float3 o_binormal : BINORMAL,
			out float2 o_texcoord : TEXCOORD0 )
{
	// Calculate position
	{
			float4 worldPos = mul( float4( i_position, 1.0), g_modelToWorldTransform );
			float4 viewPos = mul( worldPos, g_worldToViewTransform );
			float4 projectedPos = mul( viewPos, g_viewToProjectionTransform );

			o_world_position = worldPos.xyz;
			o_position = projectedPos;
	}
	//Calculate normals
	{
		float3x3 modelToWorldRotate = { g_modelToWorldTransform[0].xyz, g_modelToWorldTransform[1].xyz, g_modelToWorldTransform[2].xyz };
		float3 normal = mul( normalize( i_normal ), modelToWorldRotate );

		o_normal = normal;
		o_tangent = mul( normalize( i_tangent ), modelToWorldRotate );
		o_binormal = mul( normalize( i_binormal ), modelToWorldRotate );
	}
	// Pass texcoord
	{
		o_texcoord = i_texcoord;
	}
	// Calculate color
	{
			o_color = float4( i_color, 1.0 );
	}
}
