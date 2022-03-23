#include "Path.h"

//#include <glm/gtx/spline.hpp>

/*

	Path::Path()
	{
		spline = new Spline();
		nbr=0;
	}

	Path::~Path()
	{

	}

	void Path::addKey(Camera * cam)
	{
		glm::vec3 pos 	= cam->convertPtTo (glm::vec3(0),  	   Scene::getInstance()->getRoot()->frame());
		glm::vec3 up 	= glm::normalize(cam->convertDirTo(glm::vec3(0,1,0),  Scene::getInstance()->getRoot()->frame()));
		glm::vec3 left  = glm::normalize(cam->convertDirTo(glm::vec3(1,0,0),  Scene::getInstance()->getRoot()->frame()));
		key tmp;
		tmp.pos = pos;
		tmp.up = up;
		tmp.left = left;
		myKeys.push_back(tmp);
		spline->addPoint(pos);
		nbr++;
	}

	void Path::GetInterpolatedPos(Camera * cam,float t)
	{
		//quaternion identité
		glm::quat unity = glm::quat(1,0,0,0);
		//interpolation position courante camera
		glm::vec3 pos = spline->GetInterpolatedSplinePoint(t);
		// le delta t de position sur la courbe
		int debut = int(t*nbr);
		// les vecteur up et left à interpoler
		glm::vec3 upStart 	= myKeys[debut%myKeys.size()	].up;
		glm::vec3 upEnd 	= myKeys[(1+debut)%myKeys.size()].up;
		glm::vec3 leftStart = myKeys[debut%myKeys.size()	].left;
		glm::vec3 leftEnd 	= myKeys[(1+debut)%myKeys.size()].left;
		//calcul quaternion des up
		glm::vec3 d = glm::cross(upStart,upEnd);
		float w = 1.0f+glm::dot(upStart,upEnd);
		glm::quat transform = glm::normalize(glm::quat(w,d));
		float angle = glm::dot(upStart,upEnd);
		glm::quat current;
		// si les position sont similaire quaternion identité
			if(abs(angle)>0.001)
				current = glm::normalize(unity*glm::sin(1.0-float(t*nbr-debut))+transform*glm::sin(float(t*nbr-debut)));
			else
				current = unity;
		//Calcul quaternion des left
		glm::vec3 d2 = glm::cross(leftStart,leftEnd);
		float w2 = 1.0f+glm::dot(leftStart,leftEnd);
		glm::quat transform2 = glm::normalize(glm::quat(w2,d2));
		float angle2 = glm::dot(leftStart,leftEnd);
		glm::quat current2;
		// si les 2 quaternion sont
			if(abs(angle2)>0.001)
				current2 = glm::normalize(unity*glm::sin(1.0-float(t*nbr-debut))+transform2*glm::sin(float(t*nbr-debut)));
			else
				current2 = unity;
		//interpolation	
		glm::vec3 left 	= current2*leftStart;
		glm::vec3 up 	= current*upStart;
		glm::vec3 front = glm::cross(up,left);

//		cam->lookAt(glm::vec3(0),15.0f*pos,up);
//		cam->lookAt(15.0f*pos+front,15.0f*pos,up);
		cam->lookAt(pos+front,pos,up);
	}

int Path::getNbrPoint()
{
	return spline->getPointCount();
}

void Path::loadPath(string fileName)
{
std::ifstream myFile;
myFile.open((char*)fileName.c_str());
float px,py,pz,ux,uy,uz,lx,ly,lz;
while(myFile>>px>>py>>pz>>ux>>uy>>uz>>lx>>ly>>lz)
	{
		glm::vec3 pos 	= glm::vec3(px,py,pz);
		glm::vec3 up 	= glm::vec3(ux,uy,uz);
		glm::vec3 left  = glm::vec3(lx,ly,lz);;
		key tmp;
		tmp.pos = pos;
		tmp.up = up;
		tmp.left = left;
		myKeys.push_back(tmp);
		spline->addPoint(pos);
		nbr++;
	}
}

void Path::savePath(string fileName)
{
std::ofstream myFile;
myFile.open((char*)fileName.c_str());

int nbr = myKeys.size();
for(int i=0;i<nbr;i++)
	{
		glm::vec3 p = myKeys[i].pos;
		glm::vec3 up= myKeys[i].up;
		glm::vec3 left= myKeys[i].left;
		myFile<<p.x<<" "<<p.y<<" "<<p.z<<" "<<up.x<<" "<<up.y<<" "<<up.z<<" "<<left.x<<" "<<left.y<<" "<<left.z<<"\n";
	}	
	myFile.close();
}

void Path::draw(glm::mat4 m)
{
	//position
glBegin(GL_POINTS); 

for(float i =0;i<nbr;i+=0.1)
	{
	glm::vec4 tmp = glm::vec4(spline->GetInterpolatedSplinePoint(i),1.0)*m;
//	glVertex2f(15.0f*tmp.x/tmp.w, 15.0f*tmp.y/tmp.w);
	glVertex2f(tmp.x/tmp.w, tmp.y/tmp.w);
	}
 glEnd();
 	//up
for(float t =0;t<nbr;t+=0.1)
	{
	glm::quat unity = glm::quat(1,0,0,0);
		//interpolation position courante camera
		glm::vec3 pos = spline->GetInterpolatedSplinePoint(t);
		// le delta t de position sur la courbe
		int debut = int(t*nbr);
		// les vecteur up et left à interpoler
		glm::vec3 upStart 	= myKeys[debut%myKeys.size()	].up;
		glm::vec3 upEnd 	= myKeys[(1+debut)%myKeys.size()].up;
		glm::vec3 leftStart = myKeys[debut%myKeys.size()	].left;
		glm::vec3 leftEnd 	= myKeys[(1+debut)%myKeys.size()].left;
		//calcul quaternion des up
		glm::vec3 d = glm::cross(upStart,upEnd);
		float w = 1.0f+glm::dot(upStart,upEnd);
		glm::quat transform = glm::normalize(glm::quat(w,d));
		float angle = glm::dot(upStart,upEnd);
		glm::quat current;
		// si les position sont similaire quaternion identité
			if(abs(angle)>0.001)
				current = glm::normalize(unity*glm::sin(1.0-float(t*nbr-debut))+transform*glm::sin(float(t*nbr-debut)));
			else
				current = unity;
		//Calcul quaternion des left
		glm::vec3 d2 = glm::cross(leftStart,leftEnd);
		float w2 = 1.0f+glm::dot(leftStart,leftEnd);
		glm::quat transform2 = glm::normalize(glm::quat(w2,d2));
		float angle2 = glm::dot(leftStart,leftEnd);
		glm::quat current2;
		// si les 2 quaternion sont
			if(abs(angle2)>0.001)
				current2 = glm::normalize(unity*glm::sin(1.0-float(t*nbr-debut))+transform2*glm::sin(float(t*nbr-debut)));
			else
				current2 = unity;
		//interpolation	
		glm::vec3 left 	= current2*leftStart;
		glm::vec3 up 	= current*upStart;
		glm::vec3 front = glm::cross(up,left); 
		glm::vec4 tmp = glm::vec4(spline->GetInterpolatedSplinePoint(t),1.0)*m;
	glBegin(GL_LINES); 
glColor3f(1.0,0.0,0.0);
	glVertex2f(tmp.x/tmp.w, tmp.y/tmp.w);
	glVertex2f(tmp.x/tmp.w+up.x, tmp.y/tmp.w+up.y);
glColor3f(1.0,1.0,0.0);
	glVertex2f(tmp.x/tmp.w, tmp.y/tmp.w);
	glVertex2f(tmp.x/tmp.w+left.x, tmp.y/tmp.w+left.y);
	glEnd();
	}
	}
	*/