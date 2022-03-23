// [type] [name] [min val] [max val] [default val]
//	float m; // 0.001  0.7  0.1
float BRDFBeckmann( vec3 L, vec3 V, vec3 N, float m)
{
    vec3 H = normalize( L + V );
    float NdotH = dot(N, H);

 	float M = m*m;
    float T = NdotH*NdotH;
 	float D = exp((T-1)/(M*T)) / (3.14*M*T*T);
    return D;
}