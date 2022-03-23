#version 430

#extension GL_ARB_shading_language_include : enable
#extension GL_ARB_bindless_texture : enable

#include "/Materials/Common/Lighting/Lighting"
#line 6 

layout(std140) uniform CPU
{
	/* Les différentes textures de la planète */
	sampler2D difftext;
	sampler2D normtext;
	sampler2D heitext;
	sampler2D aotext;
	sampler2D roughtext;
};

/* Variables en entrée */

/* Couleur de l'objet */
in vec3 v_Color;
/* Normale en un sommet */
in vec3 v_Normal;
/* Position du Fragment */
in vec3 FragPos;
/* Position de la source lumineuse */
in vec3 LumPos;
/* Position de la caméra soit l'observateur de la scène */
in vec3 viewPos;
/* Coordonnees de texture venant du Vertex Shader  */ 
in vec2 v_TexCoord;

/* Variables en sortie */
layout (location = 0) out vec4 Color;

void main()
{

	/* Normal mapping */
	/* La texture contenant les normales */
	vec3 textnormal = texture(normtext,v_TexCoord).rgb * 2.0 - 1.0 ; 

	/* Couleur de la source lumineuse */
	vec3 lightColor = vec3(1.0,1.0,1.0); 

	/* Position de la source lumineuse */
	/* On peut utiliser cette variable si on souhaite definir une source de lumière dans le repère de l'objet et 
	uniquement celui-ci */
	// vec3 lightpos = vec3(0.0,30.0,0.0);

	///////////////
	/* Eclairage */
	///////////////

	/* Aspect ambiant de la lumière */
	float ambientStrength = 0.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005;
	vec3 ambient = ambientStrength * lightColor;

	/* Aspect de diffusion */

	/* On change la normale utilisée par celle contenue dans la texture */
	// vec3 norm = normalize(v_Normal);

	/* Normale contenue dans la texture contenant les normales */
	vec3 norm = normalize(textnormal); 

	/* Direction de la lumière  */
	vec3 lightdir = normalize(LumPos - FragPos);

	/* Facteur de diffusion de la lumière, avec le produit scalaire du vecteur  
	direction de la normale et du vecteur direction de la lumière */
	float diff = max(dot(norm,lightdir),0.0);

	/* Aspect diffusion de la lumière */
	vec3 diffuse = diff * lightColor;

	/* Facteur de la spécularité */
	float specularStrength = 0.5 ; 

	/* Vecteur directeur de l'observateur */
	vec3 viewDir = normalize(viewPos - FragPos);
	/* Direction de la reflextion */
	vec3 reflectDir = reflect(-lightdir,norm);

	float spec = pow(max(dot(viewDir,reflectDir),0.0),32);

	/* Aspect spéculaire de la lumière */
	vec3 specular = specularStrength * spec * lightColor;

	/* Les textures */

	vec4 texture1 = texture(difftext,v_TexCoord) ; 
	vec4 texture2 = texture(heitext,v_TexCoord) ; 
	vec4 texture3 = texture(aotext,v_TexCoord) ; 
	vec4 texture4 = texture(roughtext,v_TexCoord);

	/* On peut melanger toutes les textures */ 
	vec4 toutes_textures = texture1 * texture2 * texture3 * texture4 ; 

	/* On mélange deux textures */
	/* On n'utilise pas toutes les textures mise à disposition */ 
	/* On mélange la texture "diffuseOriginal" avec la texture "roughness" */

	vec4 deux_textures = texture1 * texture4 ;   

	/* Résultat pour l'éclairage sur les textures */
	vec3 result = (ambient + diffuse + specular) * deux_textures.xyz ;

	/* On affiche la couleur pour chaque fragment */
	Color = vec4(result,1.0) ;

}