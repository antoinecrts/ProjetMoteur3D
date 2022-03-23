#version 430
#extension GL_ARB_shading_language_include : enable
#include "/Materials/Common/Common"
layout(std140) uniform CPU
{
	mat4 M;
	mat4 NormalM;
	mat4 lastMVP;
	mat4 MVP;
};


 out gl_PerVertex {
        vec4 gl_Position;
        float gl_PointSize;
        float gl_ClipDistance[];
    };

layout (location = 0) in vec3 Position;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 TexCoord;
layout (location = 4) in vec4 Tangent;

out vec3 texCoord;
out vec3 fpos;
out vec4 tangent;
out vec3 normal;

void main()
{
	
gl_Position = ViewProj*M * vec4(Position, 1.0);
//gl_Position =  MVP*vec4(Position,1.0);
fpos = (M * vec4(Position,1.0)).xyz;
normal = (NormalM * vec4(Normal,0.0)).xyz;
tangent.xyz = (NormalM * vec4(Tangent.xyz,0.0)).xyz;
tangent.w = Tangent.w;
texCoord = TexCoord;



}
