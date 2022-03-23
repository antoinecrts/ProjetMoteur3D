#version 430

#extension GL_ARB_shading_language_include : enable
#extension GL_ARB_bindless_texture : enable

#include "/Materials/Common/Common"
#line 6 


layout(std140) uniform CPU
{
	mat4 MVP;
    /* Matrice de projection du model*/
    mat4 Model;
    /* Position du modèle de particules */
    vec3 PartPos;
    /* Position de la caméra */
    vec3 CamPos;
};

 out gl_PerVertex {
        vec4 gl_Position;
        float gl_PointSize;
        float gl_ClipDistance[];
    };

/* Les variables en entrée */

layout (location = 0) in vec3 Position;
layout (location = 2) in vec3 Normal; 
layout (location = 3) in vec2 CoulSom;
/* Tangente pour le matrice TBN */
layout (location = 4) in vec3 Tangent;  

/* Les variables en sortie */ 

out vec3 v_Color;
out vec3 v_Normal;
/* Position du fragment */
out vec3 FragPos;
/* Position de la source lumineuse */
out vec3 LumPos;
/* Position de la caméra */
out vec3 viewPos;
/* Coordonnées de la texture */
out vec2 v_TexCoord;


/* Fonction */
/* Déformation de la sphère */ 

vec3 deformation(vec3 Position) {
    vec3 newPosition;   
    if (Position.y > 0){
        newPosition.x = Position.x + 0.30 ;
    }else{
        newPosition = Position;
    } 
    return newPosition;
}


void main()
{

    /* Utilisation de la fonction de deformations */

    /* Position dans la scène */
	gl_Position = MVP * vec4(deformation(Position),1.0);

    /* Position du fragment */
    FragPos = vec3(Model * vec4(Position,1.0));

    /* Couleur de l'objet */

    /* La couleur de l'objet aura été utilisé pour effetuer des tests, l'éclairage sera 
    fait en fonction des textures appliquées sur le modèle  */

 	v_Color = vec3(1.0,0.5,0.31);

    /* Coordonnées de la texture */
    v_TexCoord = CoulSom ; 

    ////////////////////
    /* Normal mapping */
    ////////////////////

    /* Produit vectoriel normal et tangente */
    vec3 B = vec3(cross(Normal,Tangent)) ;

    /* Notre matrice de transition dans repère local à la surface */
    mat3 TBN = {
        Tangent,
        B,
        Normal
    };

    /* Emplacement de la source lumineuse */
    /* Modifié avec la matrice TBN pour le normal mapping */

    LumPos = transpose(TBN) * normalize(PartPos);
   
    /* Emplacement de la caméra, le point de vue de l'observateur */
    /* Modifié avec la matrice TBN avec le normal mapping */ 
    viewPos = transpose(TBN) * normalize(CamPos);

    /* Normal en un point */
    /* Modification avec TBN pour le normal mapping */
    /* On pourra l'utiliser si l'on souhaite ne pas utilsier la texture contenant les normales */

    v_Normal = transpose(TBN) * Normal;
   



}

