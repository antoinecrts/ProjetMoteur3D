
vec4 ParallaxOcclusionMapping(in float ParallaxDepth, in float StartDepth, in float StopDepth, in float NumSteps, in vec2 TexCoords, in vec3 V, sampler2D normalTex)
{
	float StepHeight = abs(StartDepth - StopDepth) / NumSteps; // Calcul de la profondeur parcourue à chaque itération

	vec2 SearchHeight = vec2(0.0); // Profondeur du point final : profondeur exacte entre SearchHeight.x et SearchHeight.y
	vec2 OffsetCoords = TexCoords; // Coordonées du point final

	vec2 Offset = vec2(StepHeight * ParallaxDepth * V.xy); // Facteur de déplacement dans la texture à chaque itération

	float CurrentDepth = StartDepth; // Profondeur de départ
	float Depth; // Profondeur courante

	for(float i = 0.0; i < NumSteps; ++i)
	{
		CurrentDepth -= StepHeight; // Décrémentation de la profondeur courante
		OffsetCoords += Offset; // Déplacement des coordonées de texture
		Depth = texture2D(normalTex, OffsetCoords).a; // Calcul de la profondeur aux nouvelles coordonées
		
		if(CurrentDepth <= Depth) // Si la nouvelle profondeur est inférieur ou égale à l'ancienne
			break; // Fin de boucle
	}

	OffsetCoords -= Offset; // Retour au dernières bonnes coordonées connues
	SearchHeight.x = CurrentDepth + StepHeight; // Retour à la dernière bonne profondeur connue
	SearchHeight.y = CurrentDepth; // Profondeur actuelle

	return vec4(OffsetCoords, SearchHeight); // Retour des dernières bonnes coordonées connues et de l'intervalle de profondeur dans lequel se trouve l'intersection
}

vec4 ReliefMapping(in float ParallaxDepth, in float StartDepth, in float StopDepth, in float NumSteps, in vec2 TexCoords, in vec3 V,sampler2D normalTex)
{
	vec4 POMResult = ParallaxOcclusionMapping(ParallaxDepth, StartDepth, StopDepth, NumSteps, TexCoords, V,normalTex); // Calcul de l'intervalle dans lequel se trouve l'intersection
	return ParallaxOcclusionMapping(ParallaxDepth, POMResult.z, POMResult.w, NumSteps, POMResult.xy, V,normalTex); // Calcul de la position de l'intersection dans l'intervalle
}



float SelfShadow(in float ParallaxDepth, in float StartDepth, in float StopDepth, in float NumSteps, in vec2 TexCoords, in float ShadowPower, in vec3 L,sampler2D normalTex)
{
	float StepHeight = abs(StartDepth - StopDepth) / NumSteps; // Calcul de la profondeur parcourue à chaque itération

	vec2 OffsetCoords = TexCoords; // Coordonées du point final

	vec2 Offset = vec2(StepHeight * ParallaxDepth * L.xy); // Facteur de déplacement dans la texture à chaque itération

	float CurrentDepth = StartDepth; // Profondeur de départ
	float Depth; // Profondeur courante

	float NumSubSurfaceSamples = 0.0;

	for(float i = 0.0; i < NumSteps; ++i)
	{
		CurrentDepth += StepHeight; // Décrémentation de la profondeur courante
		OffsetCoords -= Offset; // Déplacement des coordonées de texture
		Depth = texture2D(normalTex, OffsetCoords).a; // Calcul de la profondeur aux nouvelles coordonées
		if(CurrentDepth >= Depth) // Si la nouvelle profondeur est inférieur ou égale à l'ancienne
			NumSubSurfaceSamples += ShadowPower;
	}

	return (NumSubSurfaceSamples / NumSteps);
}
