#include "Spline.h"
//#include <glm/gtx/spline.hpp>

float modulo(float x, float y)
    {
        /*x modulo y*/
        x-=y*abs(x/y);
        if (x>=0.) return (x);
        else return (x+y);
    }


glm::vec3 catmullRom(glm::vec3 a,glm::vec3 b,glm::vec3 c,glm::vec3 d,float t)
	{
		const float s = t, s1 = t,s2=t*t,s3=t*t*t;
		const float f1 = -s3+2*s2-s;
		const float f2 = 3*s3-5*s2+2;
		const float f3 = -3*s3+4*s2+s;
		const float f4 = s3-s2;
		return 0.5f*(f1*a+f2*b+f3*c+f4*d);
	}

	Spline::Spline()
	{

	}

	Spline::~Spline()
	{

	}

	void Spline::addPoint(glm::vec3 p)
	{
		myPoints.push_back(p);
	}

	glm::vec3 Spline::GetInterpolatedSplinePoint(float t)
	{
		int nbr = getPointCount();
		glm::mat4 mat = glm::mat4(glm::vec4(0,-1,2,-1),glm::vec4(2,0,-5,3),glm::vec4(0,1,4,-3),glm::vec4(0,0,-1,1));
		glm::vec4 T = glm::vec4(1.0,t,t*t,t*t*t);
		if(t<0 || nbr==0) 
			return glm::vec3(0);// error case no point or t<0
		else
			{
			int debut = int(t*nbr);
			return catmullRom(myPoints[(debut)%nbr],myPoints[(debut+1)%nbr],myPoints[(debut+2)%nbr],myPoints[(debut+3)%nbr],t*nbr-debut); ///do the job	
			}
			
	}
	
	int Spline::getPointCount()
	{
		return myPoints.size();
	}
/*
	glm::vec3 Spline::GetInterpolatedSplinePoint(float t)
	{
		int nbr = getPointCount();
		glm::mat4 mat = glm::mat4(glm::vec4(0,-1,2,-1),glm::vec4(2,0,-5,3),glm::vec4(0,1,4,-3),glm::vec4(0,0,-1,1));
		glm::vec4 T = glm::vec4(1.0,t,t*t,t*t*t);
		if(t<0 || nbr==0) 
			return glm::vec3(0);// error case no point or t<0
		else
			{
			int debut = int(t*nbr);
			return catmullRom(myPoints[(debut)%nbr],myPoints[(debut+1)%nbr],myPoints[(debut+2)%nbr],myPoints[(debut+3)%nbr],t*nbr-debut); ///do the job	
//			return glm::gtx::spline::catmullRom(myPoints[(debut)%nbr],myPoints[debut+1],myPoints[debut+2],myPoints[debut+3],t*nbr-debut); ///do the job	
			}
			
	}
*/	