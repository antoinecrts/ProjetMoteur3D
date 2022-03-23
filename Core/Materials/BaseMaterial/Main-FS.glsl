#version 430

#extension GL_ARB_shading_language_include : enable
#include "/Materials/Common/Lighting/Lighting"
#line 6 





in vec3 v_Normal;
in vec3 fpos;

layout (location = 0) out vec4 Color;

void main()
{
vec4 col = vec4(0.8,0.0,0.8,1.0);
	
	
Color = col;
}