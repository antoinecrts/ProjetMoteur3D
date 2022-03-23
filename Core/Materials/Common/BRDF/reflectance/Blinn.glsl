// [type] [name] [min val] [max val] [default val]
//    float n; // 1  100  10
//    float ior; // 1  2.5  1.5
//    bool include_Fresnel; // false  true  false
//    bool divide_by_NdotL; // false  true  true
float BRDFBlinn( vec3 L, vec3 V, vec3 N, float n, float ior, bool include_Fresnel, bool divide_by_NdotL)
{   
    vec3 H = normalize(L+V);

    float NdotH = dot(N, H);
    float VdotH = dot(V, H);
    float NdotL = dot(N, L);
    float NdotV = dot(N, V);

    float x = acos(NdotH) * n;
    float D = exp( -x*x);
    float G = (NdotV < NdotL) ? 
        ((2*NdotV*NdotH < VdotH) ?
         2*NdotH / VdotH :
         1.0 / NdotV)
        :
        ((2*NdotL*NdotH < VdotH) ?
         2*NdotH*NdotL / (VdotH*NdotV) :
         1.0 / NdotV);

    // fresnel
    float c = VdotH;
    float g = sqrt(ior*ior + c*c - 1);
    float F = 0.5 * pow(g-c,2) / pow(g+c,2) * (1 + pow(c*(g+c)-1,2) / pow(c*(g-c)+1,2));

    float val = NdotH < 0 ? 0.0 : D * G * (include_Fresnel ? F : 1.0);

    if (divide_by_NdotL)
        val = val / dot(N,L);
    return val;
}