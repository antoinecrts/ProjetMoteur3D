#version 420

/* Texture contenue dans le FBO */
uniform sampler2D fboIn;

/* Variables en entrée */ 
in vec3 texCoord;

/* Variables en sortie */ 
layout (location = 0) out vec4 Color;


/* La résolution du FBO est de 2048 par 2048 */
/* On dit que pour passer au pixel voisin il faut faire +- 1.0 */
const float blurSizeH = 1.0 / 2048.0;
const float blurSizeV = 1.0 / 2048.0;


void main()
{
	/* On effectue la moyenne des pixels voisins */
	/* Chaque pixel va regarder ses voisins */
	/* On défini un floutage où on aura 9 pixels "regardés" */
    vec4 sum = vec4(0.0);
    for (int x = -3; x <= 3; x++)
        for (int y = -3; y <= 3; y++)
            sum += texture(fboIn,vec2(texCoord.x + x * blurSizeH,texCoord.y + y * blurSizeV)) / 49.0;

    Color = sum;

}
