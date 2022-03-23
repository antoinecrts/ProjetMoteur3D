// [type] [name] [min val] [max val] [default val]
//	float Rs; // 0  1  0.1
//	float Rd; // 0  1  1
//	float nu; // 1  1000  100
//	float nv; // 1  1000  100
//	bool isotropic; // false  true  true
//	bool coupled_diffuse; // false  true  true
//	X et Y tengente et bitengente a la surface
float BRDFAshikhmanShirley( vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y, float Rs, float Rd, float nu, float nv, bool isotropic, bool coupled_diffuse)
{   
	float PI = 3.14159265358979323846;
    vec3 H = normalize(L+V);
    float HdotV = dot(H,V);
    float HdotX = dot(H,X);
    float HdotY = dot(H,Y);
    float NdotH = dot(N,H);
    float NdotV = dot(N,V);
    float NdotL = dot(N,L);
    
    float F = Rs + (1-Rs) * pow(1-HdotV, 5);
    float norm_s = sqrt((nu+1)*((isotropic?nu:nv)+1))/(8*PI);
    float n = isotropic ? nu :(nu*HdotX*HdotX + nv*HdotY*HdotY)/(1-NdotH*NdotH);
    float rho_s = norm_s * F * pow(max(NdotH,0), n) / (HdotV * max(NdotV, NdotL));

    float rho_d = 28/23 * Rd * (1-pow(1-NdotV/2, 5)) * (1-pow(NdotL/2, 5));
    if (coupled_diffuse) rho_d *= (1-Rs);

    return (rho_s + rho_d)*dot(N,L);
}