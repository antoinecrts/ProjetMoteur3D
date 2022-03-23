/*#ifndef __PATH__
#define __PATH__

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Spline.h"
#include "Camera.h"
#include "Scene.h"
#include <vector>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include <fstream>


struct key
{
	glm::vec3 pos;
	glm::vec3 left;
	glm::vec3 up;
};

class Path
{
public:
	Path();
	~Path();
	void addKey(Camera * cam);
	void GetInterpolatedPos(Camera * cam, float t);
	int getNbrPoint();
	void draw(glm::mat4 &m);
	void savePath(string fileName);
	void loadPath(string fileName);
private:
	std::vector<key> myKeys;
	Spline * spline;
	int nbr;
};

#endif

*/