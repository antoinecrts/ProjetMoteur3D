// [type] [name] [min val] [max val] [default val]
//    float Kd; // 0  1  1
//    float Ks; // 0  1  0.1
//    float alphaG; // 0.001  1  0.1
//    float ior; // 1.3  3  2
//    bool useFresnel; // false  true  false
float BRDFWalter( vec3 L, vec3 V, vec3 N, float Kd, float Ks, float alphaG, float ior, bool useFresnel)
{   
    float NdotL = dot(N, L);
    float NdotV = dot(N, V);
    if (NdotL < 0 || NdotV < 0) return 0;

    vec3 H = normalize(L+V);
    float NdotH = dot(N, H);
    float VdotH = dot(V, H);

    float D = alphaG*alphaG / ((NdotH*NdotH*(alphaG*alphaG-1) + 1)*(NdotH*NdotH*(alphaG*alphaG-1) + 1));
    float G = 2/(1 + sqrt(1 + alphaG*alphaG * (1-NdotL*NdotL)/(NdotL*NdotL)));
    G=G*2/(1 + sqrt(1 + alphaG*alphaG * (1-NdotV*NdotV)/(NdotV*NdotV)));

    // fresnel
    float c = VdotH;
    float g = sqrt(ior*ior + c*c - 1);
    float F = useFresnel ? 0.5 * pow(g-c,2) / pow(g+c,2) * (1 + pow(c*(g+c)-1,2) / pow(c*(g-c)+1,2)) : 1.0;

    float val = Kd + Ks * D * G * F / (4 * NdotL * NdotV);
    return val;
}