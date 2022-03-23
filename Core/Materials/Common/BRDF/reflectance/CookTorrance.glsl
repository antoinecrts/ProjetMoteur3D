// [type] [name] [min val] [max val] [default val]
//  float m; // 0.001  0.3  0.1
//  float f0; // 0  1  0.1
//  bool include_F; //  false  true  true
//  bool include_G; //  false  true  true

float sqr(float x) { return x*x; }

float Beckmann(float m, float t)
{
    float M = m*m;
    float T = t*t;
    return exp((T-1)/(M*T)) / (M*T*T);
}

float Fresnel(float f0, float u)
{
    // from Schlick
    return f0 + (1-f0) * pow(1-u, 5);
}

float BRDFCookTorrance( vec3 L, vec3 V, vec3 N, float m, float f0, bool include_F, bool include_G)
{   
    // compute the half vec3
    vec3 H = normalize( L + V );

    float NdotH = dot(N, H);
    float VdotH = dot(V, H);
    float NdotL = dot(N, L);
    float NdotV = dot(N, V);
    float oneOverNdotV = 1.0 / NdotV;

    float D = Beckmann(m, NdotH);
    float F = Fresnel(f0, VdotH);

    NdotH = NdotH + NdotH;
    float G = (NdotV < NdotL) ? 
        ((NdotV*NdotH < VdotH) ?
         NdotH / VdotH :
         oneOverNdotV)
        :
        ((NdotL*NdotH < VdotH) ?
         NdotH*NdotL / (VdotH*NdotV) :
         oneOverNdotV);

    if (include_G) G = oneOverNdotV;
    float val = NdotH < 0 ? 0.0 : D * G ;

    if (include_F) val *= F;

    val = val / NdotL;
    return val;
}