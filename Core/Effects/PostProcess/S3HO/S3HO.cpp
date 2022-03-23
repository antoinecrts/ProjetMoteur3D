#include "S3HO.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Engine.h"







//void TW_CALL SwitchDSSDO(void * /*clientData*/) {
//	active = !active;
//}
//









S3HO::S3HO(std::string name, int width, int height) :
	EffectGL(name,"PostProcess/S3HO")
{

	active = true;
	noiseTex = NULL;



	fbo_width = width;
	fbo_height = height;

	Base = new GLProgram(this->m_ClassName + "-Base", GL_VERTEX_SHADER);
	m_ProgramPipeline->useProgramStage(GL_VERTEX_SHADER_BIT, Base);
		
	DO_PRG_DO = new GLProgram(this->m_ClassName + "-SSDO", GL_FRAGMENT_SHADER);
	DO_PRG_DO_HBlur = new GLProgram(this->m_ClassName + "-SSDO-HBlur", GL_FRAGMENT_SHADER);
	DO_PRG_DO_VBlur = new GLProgram(this->m_ClassName + "-SSDO-VBlur", GL_FRAGMENT_SHADER);

	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DO_PRG_DO);
	m_ProgramPipeline->link();
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DO_PRG_DO_HBlur);
	m_ProgramPipeline->link();
	m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DO_PRG_DO_VBlur);
	m_ProgramPipeline->link();



	//default values
	noiseTexture_numSamples = 64;
	noiseSize = 16;
	doBaseRadiusVal = 0.0017;
	doMaxOcclusionDistVal = 4.81f;


	createParameters();

	createFBO();
	

	createSamplers();
	computeNoiseTexture();
	computeKernels();
	



}


void S3HO::createFBO()
{
	DO_FBO_DO = new GPUFBO("DO_FBO_DO");
	DO_FBO_DO_Blur[0] = new GPUFBO("DO_FBO_DO_HBlur");
	DO_FBO_DO_Blur[1] = new GPUFBO("DO_FBO_DO_VBlur");

	DO_FBO_DO->createTexture2DAttachments(FBO_WIDTH, FBO_HEIGHT, 1, false, GL_RGB16F);
	DO_FBO_DO_Blur[0]->createTexture2DAttachments(FBO_WIDTH, FBO_HEIGHT, 1, false, GL_RGB16F);
	DO_FBO_DO_Blur[1]->createTexture2DAttachments(FBO_WIDTH, FBO_HEIGHT, 1, false, GL_RGB16F);
}
void S3HO::createSamplers()
{

	DO_Sampler_DO_GBuffers = DO_PRG_DO->uniforms()->getGPUsampler("GBuffers");
	DO_Sampler_DO_Depth = DO_PRG_DO->uniforms()->getGPUsampler("depthSampler");
	DO_Sampler_DO_Noise = DO_PRG_DO->uniforms()->getGPUsampler("noiseSampler");

	DO_Sampler_DO_HBLUR_GBuffers = DO_PRG_DO_HBlur->uniforms()->getGPUsampler("GBuffers");
	DO_Sampler_DO_HBlur_DO = DO_PRG_DO_HBlur->uniforms()->getGPUsampler("DOSampler");
	DO_Sampler_DO_VBLUR_GBuffers = DO_PRG_DO_VBlur->uniforms()->getGPUsampler("GBuffers");
	DO_Sampler_DO_VBlur_DO = DO_PRG_DO_VBlur->uniforms()->getGPUsampler("DOSampler");

	DO_Sampler_DO_GBuffers->Set(0);
	DO_Sampler_DO_Depth->Set(1);
	

	DO_Sampler_DO_HBLUR_GBuffers->Set(0);
	DO_Sampler_DO_HBlur_DO->Set(1);
	DO_Sampler_DO_VBLUR_GBuffers->Set(0);
	DO_Sampler_DO_VBlur_DO->Set(1);


	numSamples->Set(noiseTexture_numSamples);
}


void S3HO::createParameters()
{
	
	doHStep = DO_PRG_DO->uniforms()->getGPUfloat( "HStep" );
	doVStep = DO_PRG_DO->uniforms()->getGPUfloat( "VStep" );
	numSamples = DO_PRG_DO->uniforms()->getGPUint( "numSamples" );

	doBaseRadius = DO_PRG_DO->uniforms()->getGPUfloat( "baseRadius" );
	doMaxOcclusionDist = DO_PRG_DO->uniforms()->getGPUfloat( "maxOcclusionDist" );

	doHBlurSize = DO_PRG_DO_HBlur->uniforms()->getGPUint( "doBlurSize" );
	doHBlurHStep = DO_PRG_DO_HBlur->uniforms()->getGPUfloat( "HStep" );

	doVBlurSize = DO_PRG_DO_VBlur->uniforms()->getGPUint("doBlurSize");
	doVBlurVStep = DO_PRG_DO_VBlur->uniforms()->getGPUfloat( "VStep" );





	
}
void S3HO::setUpParameters()
{
	doHBlurHStep->Set(1.0f / DO_FBO_DO_Blur[0]->getWidth());
	doVBlurVStep->Set(1.0f / DO_FBO_DO_Blur[1]->getHeight());
	numSamples->Set(noiseTexture_numSamples);
	doHStep->Set(DO_FBO_DO->getWidth() / (float)noiseSize);
	doVStep->Set(DO_FBO_DO->getHeight() / (float)noiseSize);
	doHBlurSize->Set(noiseSize);
	doVBlurSize->Set(noiseSize);

	DO_Sampler_DO_Noise->Set(noiseTex->getHandle());
}


void S3HO::createControlBar()
{
//	controlBar = TwNewBar("S3HO Control Bar");
	//TwAddVarRW(controlBar, "DSSDO Base Radius", TW_TYPE_FLOAT, &doBaseRadiusVal, " min=0.0001 max=2 step=0.0001 group='DSSDO' label='Radius' ");
//	TwAddVarRW(controlBar, "DSSDO Max Occlusion Dist", TW_TYPE_FLOAT, &doMaxOcclusionDistVal, " min=0.001 max=40.0 step=0.01 group='DSSDO' label='Max' ");
}





S3HO::~S3HO()
{
	
	delete Base;
	delete noiseTex;
}



void S3HO::apply(GPUFBO *GBuffers, GPUFBO* normalizedDepth, GPUFBO* out)
{

	
	//if (active)
	{
		//----- Ambient Occlusion -----//
		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DO_PRG_DO);

		doBaseRadius->Set(doBaseRadiusVal);
		doMaxOcclusionDist->Set(doMaxOcclusionDistVal);

		DO_FBO_DO->enable();
		//out->enable();
		GBuffers->getColorTexture(0)->bind(DO_Sampler_DO_GBuffers->getValue());
		normalizedDepth->getColorTexture(0)->bind(DO_Sampler_DO_Depth->getValue());
		this->drawQuad();	
		normalizedDepth->getColorTexture(0)->release();
		GBuffers->getColorTexture(0)->release();
		DO_FBO_DO->disable();
		//out->disable();

		//----- Blur Pass -----//
		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DO_PRG_DO_HBlur);

		//AO_FBO_AO_Blur[0]->enable();
		DO_FBO_DO_Blur[0]->enable();
		GBuffers->getColorTexture(0)->bind(DO_Sampler_DO_HBLUR_GBuffers->getValue());
		DO_FBO_DO->getColorTexture(0)->bind(DO_Sampler_DO_HBlur_DO->getValue());
		this->drawQuad();
		DO_FBO_DO->getColorTexture(0)->release();
		GBuffers->getColorTexture(0)->release();
		DO_FBO_DO_Blur[0]->disable();



		m_ProgramPipeline->useProgramStage(GL_FRAGMENT_SHADER_BIT, DO_PRG_DO_VBlur);

		if (out != NULL)
			out->enable();
		GBuffers->getColorTexture(0)->bind(DO_Sampler_DO_VBLUR_GBuffers->getValue());
		DO_FBO_DO_Blur[0]->getColorTexture(0)->bind(DO_Sampler_DO_VBlur_DO->getValue());

		this->drawQuad();

		DO_FBO_DO_Blur[0]->getColorTexture(0)->release();
		GBuffers->getColorTexture(0)->release();

		if (out != NULL)
			out->disable();
	}
	

}


void S3HO::computeKernels()
{
	srand(time(NULL));

	//----- Samples Kernel Generation -----//
	float scale;
	unsigned int kernelSize = noiseTexture_numSamples;
	glm::vec4 *kernel = new glm::vec4[kernelSize];


	kernels = new GPUBuffer("KernelsBuffer");
	kernels->create(sizeof(glm::vec4)*kernelSize, GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW);
	kernels->setBindingPoint(7);
	glm::vec4* ptr = (glm::vec4 *) kernels->mapRange(0, kernelSize*sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);


	glm::vec4 pp;
		for (unsigned int i = 0; i < kernelSize; ++i)
		{	
			
			pp.x = ((-100.0f + (float)(rand() % 200)) / 100.0f);
			pp.y = ((-100.0f + (float)(rand() % 200)) / 100.0f);
			pp.z = ((-100.0f + (float)(rand() % 200)) / 100.0f);

			// Normalize the random vector to fall on the unit hemisphere
			pp = glm::normalize(pp);
			scale = float(i) / float(kernelSize);
			scale = glm::mix(0.1f, 1.0f, scale * scale);
			pp *= scale;
			ptr[i] = pp;
		}
	//	
		kernels->unMap();

		setUpParameters();
}



void S3HO::computeNoiseTexture()
{
	

	//----- Noise Texture Generation -----//
	unsigned int noiseTexSizeX = noiseSize;
	unsigned int noiseTexSizeY = noiseSize;
	unsigned int noiseSize = noiseTexSizeX * noiseTexSizeY;
	glm::vec3 *noise = new glm::vec3[noiseSize];

	float thresold = 0.125f;
	for (unsigned int i = 0; i < noiseSize; ++i)
	{
		// Generating random points in z oriented hemisphere
		noise[i].x = ((-1000.0f + (float)(rand() % 2000)) / 1000.0f);
		noise[i].y = ((-1000.0f + (float)(rand() % 2000)) / 1000.0f);
		noise[i].z = 0.0f;

		// Normalize the random vector to fall on the unit hemisphere
		noise[i] = glm::normalize(noise[i]);

		if ((noise[i].x > -thresold && noise[i].x < thresold) || (noise[i].y > -thresold && noise[i].y < thresold))
			i = --i;
	}

	// Set Up noise texture

	if (noiseTex != NULL)
		delete noiseTex;

	noiseTex = new GPUTexture2D("NoiseTexture", noiseTexSizeX, noiseTexSizeY, 3, (float*)noise);


	GLuint noiseSamplerID;
	// Set Up noise sampler 
	glGenSamplers(1, &noiseSamplerID);
	glBindSampler(noiseTex->getId(), noiseSamplerID);
	glSamplerParameteri(noiseSamplerID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(noiseSamplerID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(noiseSamplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(noiseSamplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);





	setUpParameters();
	delete[] noise;
}

