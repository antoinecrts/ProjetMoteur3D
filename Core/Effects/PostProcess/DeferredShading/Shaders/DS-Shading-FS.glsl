/*
*	Authors: G. THOIN
*	Master 2 ISICG
*/

#version 440 core
#extension GL_ARB_shading_language_include : enable
#include "/Materials/Common/Lighting/Lighting"
#include "/Materials/Common/Common"

uniform sampler2DArray GBuffers;

in vec2 texCoord;
layout (location = 0) out vec3 Color;



void main()
{
	
	//----- Albedo : Diffuse -----//
	vec3 BaseColor = texture(GBuffers, vec3(texCoord.xy, 1.0)).rgb;
	//----- Specular -----//
	vec3 MetalMask_Reflectance_Roughness = texture(GBuffers, vec3(texCoord.xy, 2.0)).rgb;
	//----- Position -----//
	vec3 Position = texture(GBuffers, vec3(texCoord.xy, 3.0)).xyz;
	//----- Normal -----//
	vec3 N = normalize(texture(GBuffers, vec3(texCoord.xy, 4.0)).xyz);
	
	float metalMask = MetalMask_Reflectance_Roughness.r;
	float reflectance = MetalMask_Reflectance_Roughness.g;
	float roughness = MetalMask_Reflectance_Roughness.b;

	vec3 F0 = 0.16f*(reflectance*reflectance)*(1.0f - metalMask) + BaseColor*metalMask;
	
	Color = addPBR( Position, N,  BaseColor, roughness, F0,vec3(0.0)).rgb;

	
}	