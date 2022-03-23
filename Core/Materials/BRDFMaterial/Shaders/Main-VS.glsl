#version 430

#extension GL_ARB_shading_language_include : enable
#include "/Materials/Common/Lighting/Lighting"



#line 6 


layout(std140) uniform CPU
{
    mat4 MVP;
	mat4 MV;
	mat4 NormalMV;
    
	
};

 out gl_PerVertex {
        vec4 gl_Position;
        float gl_PointSize;
        float gl_ClipDistance[];
    };



out vec3 N;
out vec3 pointPosition;
out vec3 pos;
out vec3 V;

layout (location = 0) in vec3 Position;
layout (location = 2) in vec3 Normale;


void main()
{
	N = normalize((NormalMV * vec4(Normale, 0.0)).xyz);

    
    
	gl_Position = MVP * vec4(Position,1.0) ;
	pointPosition = (MV * vec4(Position, 1.0)).xyz;
	pos = Position;
		
	V = normalize(camPos.xyz);
	
}