#ifndef __SPLINE__
#define __SPLINE__

#include <glm/glm.hpp>

#include <vector>


class Spline
{
public:
	Spline();
	~Spline();
	void addPoint(glm::vec3 p);
	glm::vec3 GetInterpolatedSplinePoint(float t);
	int getPointCount();
//	glm::vec3 Spline::catmullRom(glm::vec3 a,glm::vec3 b,glm::vec3 c,glm::vec3 d,float t);
private:
	std::vector<glm::vec3> myPoints;
	float dt;
};

#endif

