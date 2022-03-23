float phongBRDF(vec3 N, vec3 V, vec3 L, float spec_pow){
	vec3 R = normalize(reflect(-L,N));
    float val = pow(max(0, dot(R,V)),spec_pow);
    val = val / dot(N,L);
    return val;
}