// [type] [name] [min val] [max val] [default val]
//	float c; // 0.001  1  0.1
//	bool normalized; // false  true
float BRDFTrowbridge( vec3 L, vec3 V, vec3 N, float c, bool normalized)
{   
    vec3 H = normalize( L + V );
    float D = sqrt(c*c / (dot(N,H)*dot(N,H)*(c*c-1)+1));
    if (normalized) {
        D *= min(1,1/(c*c*3.14));
    }
    return D;
}