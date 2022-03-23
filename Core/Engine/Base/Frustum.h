#ifndef __FRUSTUM__
#define __FRUSTUM__

#include <glm/glm.hpp>
#include <iostream>
#include "GPUResources/Buffers/GPUBuffer.h"
#include <string>


struct plan;
struct arretes;

struct point
{
	glm::vec3 pos;
	plan *p1,*p2,*p3;
	string name;
};


struct arretes
{
	point* d;
	point* f;
	int cpt = 2;
	int dad;
	plan *p1,*p2;
	string name;
//	glm::vec3
};

struct plan
{
	arretes *un, *deux,*trois,*quatre;
	glm::vec4 equation;
	bool toKeep = true;
	string name;
};


class Frustum{
public:
	Frustum();
	void computeConvex(glm::vec3 l);
	void update(const glm::mat4 &matrix);
//	void draw(glm::mat4 matrix, int n);
	void getEdge(int indice, glm::vec3 * v1, glm::vec3 * v2);
private:	
	plan plane[15];
	point ndcPoint[8];
	arretes arretesList[12];
	glm::vec4 convex[100];
	GPUBuffer *GPlanes;	
	glm::vec3 inside;

};

#endif