 #include "Frustum.h"

	#define R 0
	#define L 1
	#define B 2
	#define T 3
	#define F 4
	#define N 5

glm::vec3 cptCorner(glm::vec4 &p1, glm::vec4 &p2, glm::vec4 &p3)
{
	glm::mat3 t;
	t[0]=glm::vec3(p1.x,p2.x,p3.x);
	t[1]=glm::vec3(p1.y,p2.y,p3.y);
	t[2]=glm::vec3(p1.z,p2.z,p3.z);
	glm::vec3 d = -glm::vec3(p1.w,p2.w,p3.w);
	return (glm::inverse(t))*d;
}


glm::vec4 cptPlane2(glm::vec3 a,glm::vec3 b,glm::vec3 c)
{

	glm::vec3 n = glm::cross(a-b,c-b);
	float w = glm::dot(n,a);
	return glm::vec4(n,-w);
}

bool insidePlane2(glm::vec4 &p, glm::vec3 &l)
{
	return glm::sign(l.x*p.x+l.y*p.y+l.z*p.z+p.w)>0;
}

using namespace glm;

void Frustum::update(const glm::mat4 &matrix)
{
 	for(uint i = 0;i<12;i++)	arretesList[i].cpt=2;

 	float norme;
 	vec3 normal;
 	
 	plane[L].equation 	= normalize(glm::vec4( matrix[0][3] + matrix[0][0],matrix[1][3] + matrix[1][0],matrix[2][3] + matrix[2][0],matrix[3][3] + matrix[3][0] ));
 	normal = vec3( plane[L].equation.x, plane[L].equation.y, plane[L].equation.z ); 
 	norme = length(normal);
 	plane[L].equation   = vec4( normalize(normal), plane[L].equation.w/norme );

	plane[R].equation 	= normalize(vec4( matrix[0][3] - matrix[0][0],matrix[1][3] - matrix[1][0],matrix[2][3] - matrix[2][0],matrix[3][3] - matrix[3][0] ));
	normal = vec3( plane[R].equation.x, plane[R].equation.y, plane[R].equation.z ); 
 	norme = length(normal);
 	plane[R].equation   = vec4( normalize(normal), plane[R].equation.w/norme );

	plane[T].equation 	= normalize(vec4( matrix[0][3] - matrix[0][1],matrix[1][3] - matrix[1][1],matrix[2][3] - matrix[2][1],matrix[3][3] - matrix[3][1] ));
	normal = vec3( plane[T].equation.x, plane[T].equation.y, plane[T].equation.z ); 
 	norme = length(normal);
 	plane[T].equation   = vec4( normalize(normal), plane[T].equation.w/norme );

 	plane[B].equation 	= normalize(vec4( matrix[0][3] + matrix[0][1],matrix[1][3] + matrix[1][1],matrix[2][3] + matrix[2][1],matrix[3][3] + matrix[3][1] ));
	normal = vec3( plane[B].equation.x, plane[B].equation.y, plane[B].equation.z ); 
 	norme = length(normal);
 	plane[B].equation   = vec4( normalize(normal), plane[B].equation.w/norme );

 	plane[F].equation 	= normalize(vec4( matrix[0][3] - matrix[0][2],matrix[1][3] - matrix[1][2],matrix[2][3] - matrix[2][2],matrix[3][3] - matrix[3][2] ));
	normal = vec3( plane[F].equation.x, plane[F].equation.y, plane[F].equation.z ); 
 	norme = length(normal);
 	plane[F].equation   = vec4( normalize(normal), plane[F].equation.w/norme );

 	plane[N].equation 	= normalize(vec4( matrix[0][3] + matrix[0][2],matrix[1][3] + matrix[1][2],matrix[2][3] + matrix[2][2],matrix[3][3] + matrix[3][2] ));
	normal = vec3( plane[N].equation.x, plane[N].equation.y, plane[N].equation.z ); 
 	norme = length(normal);
 	plane[N].equation   = vec4( normalize(normal), plane[N].equation.w/norme );

	plane[L].name 	="Left";
	plane[R].name	="Right";
	plane[T].name	="Top";
	plane[B].name	="Bottom";
	plane[F].name	="Far";
	plane[N].name	="Near";


	ndcPoint[0].pos =  cptCorner(plane[N].equation,plane[L].equation,plane[T].equation);//av haut gauche 
	ndcPoint[0].p1  =  &plane[N];
	ndcPoint[0].p2  =  &plane[L];
	ndcPoint[0].p3  =  &plane[T];
	ndcPoint[0].name= "NLT";

	ndcPoint[1].pos =  cptCorner(plane[N].equation,plane[R].equation,plane[T].equation);//av haut droit
	ndcPoint[1].p1  =  &plane[N];
	ndcPoint[1].p2  =  &plane[R];
	ndcPoint[1].p3  =  &plane[T];
	ndcPoint[1].name= "NRT";

	ndcPoint[2].pos =  cptCorner(plane[N].equation,plane[R].equation,plane[B].equation);//av bas  droit
	ndcPoint[2].p1  =  &plane[N];
	ndcPoint[2].p2  =  &plane[R];
	ndcPoint[2].p3  =  &plane[B];
	ndcPoint[2].name= "NRB";

	ndcPoint[3].pos =  cptCorner(plane[N].equation,plane[L].equation,plane[B].equation);//av bas  gauche
	ndcPoint[3].p1  =  &plane[N];
	ndcPoint[3].p2  =  &plane[L];
	ndcPoint[3].p3  =  &plane[B];
	ndcPoint[3].name= "NLB";

	ndcPoint[4].pos =  cptCorner(plane[F].equation,plane[L].equation,plane[T].equation);//ar haut gauche
	ndcPoint[4].p1  =  &plane[F];
	ndcPoint[4].p2  =  &plane[L];
	ndcPoint[4].p3  =  &plane[T];
	ndcPoint[4].name= "FLT";

	ndcPoint[5].pos =  cptCorner(plane[F].equation,plane[R].equation,plane[T].equation);//ar haut droit
	ndcPoint[5].p1  =  &plane[F];
	ndcPoint[5].p2  =  &plane[R];
	ndcPoint[5].p3  =  &plane[T];
	ndcPoint[5].name= "FRT";

	ndcPoint[6].pos =  cptCorner(plane[F].equation,plane[R].equation,plane[B].equation);//ar bas  droit
	ndcPoint[6].p1  =  &plane[F];
	ndcPoint[6].p2  =  &plane[R];
	ndcPoint[6].p3  =  &plane[B];
	ndcPoint[6].name= "FRB";


	ndcPoint[7].pos =  cptCorner(plane[F].equation,plane[L].equation,plane[B].equation);//ar bas  gauche
	ndcPoint[7].p1  =  &plane[F];
	ndcPoint[7].p2  =  &plane[L];
	ndcPoint[7].p3  =  &plane[B];
	ndcPoint[7].name= "FLB";


	for(int i=0; i<8; i++)
		inside += ndcPoint[i].pos;
	inside = vec3(inside.x/8.0,inside.y/8.0,inside.z/8.0);

	arretesList[0].d  = &ndcPoint[0];// top near
	arretesList[0].f  = &ndcPoint[1];//
	plane[N].un    	  = &arretesList[0];// 
	plane[T].un       =&arretesList[0]; 
	arretesList[0].p1 = &plane[N];
	arretesList[0].p2 = &plane[T];
	arretesList[0].name = "TN";

	arretesList[1].d = &ndcPoint[1];// right near  OK
	arretesList[1].f = &ndcPoint[2];
	plane[N].deux  =&arretesList[1];//
	plane[R].quatre=&arretesList[1];
	arretesList[1].p1 = &plane[R];
	arretesList[1].p2 = &plane[N];
	arretesList[1].name = "RN";

	arretesList[2].d = &ndcPoint[2];// near bot  
	arretesList[2].f = &ndcPoint[3];
	plane[N].trois =&arretesList[2];//
	plane[B].un     =&arretesList[2]; 
	arretesList[2].p1 = &plane[B];
	arretesList[2].p2 = &plane[N];
	arretesList[2].name = "NB";

	arretesList[3].d = &ndcPoint[3];// left near  OK
	arretesList[3].f = &ndcPoint[0];
	plane[L].un    =&arretesList[3]; 
	plane[N].quatre=&arretesList[3];//
	arretesList[3].p1 = &plane[N];
	arretesList[3].p2 = &plane[L];
	arretesList[3].name = "LN";

	arretesList[4].d = &ndcPoint[4];// top far
	arretesList[4].f = &ndcPoint[5];
	plane[F].un    =&arretesList[4]; 
	plane[T].trois =&arretesList[4];
	arretesList[4].p1 = &plane[T];
	arretesList[4].p2 = &plane[F];
	arretesList[4].name = "TF";

	arretesList[5].d = &ndcPoint[5];// far right   OK
	arretesList[5].f = &ndcPoint[6];
	plane[F].deux  =&arretesList[5];
	plane[R].deux  =&arretesList[5];
	arretesList[5].p1 = &plane[F];
	arretesList[5].p2 = &plane[R];
    arretesList[5].name = "FR";

	arretesList[6].d = &ndcPoint[6];// far bot
	arretesList[6].f = &ndcPoint[7];
	plane[F].trois =&arretesList[6];
	plane[B].trois =&arretesList[6];
	arretesList[6].p1 = &plane[F];
	arretesList[6].p2 = &plane[B];
	arretesList[6].name = "FB";

	arretesList[7].d = &ndcPoint[7];// far left  OK
	arretesList[7].f = &ndcPoint[4];
	plane[F].quatre =&arretesList[7];
	plane[L].trois =&arretesList[7];
	arretesList[7].p1 = &plane[L];
	arretesList[7].p2 = &plane[F];
	arretesList[7].name = "FL";

	arretesList[8].d = &ndcPoint[0];// left top
	arretesList[8].f = &ndcPoint[4];
	plane[L].deux  =&arretesList[8];
	plane[T].deux   =&arretesList[8];
	arretesList[8].p1 = &plane[L];
	arretesList[8].p2 = &plane[T];
	arretesList[8].name = "LT";	

	arretesList[9].d = &ndcPoint[1];// top right
	arretesList[9].f = &ndcPoint[5];
	plane[T].quatre=&arretesList[9];
	plane[R].un  =&arretesList[9]; 
	arretesList[9].p1 = &plane[T];
	arretesList[9].p2 = &plane[R];
	arretesList[9].name = "TR";

	arretesList[10].d = &ndcPoint[2];// bot right
	arretesList[10].f = &ndcPoint[6];
	plane[R].trois =&arretesList[10];
	plane[B].deux    =&arretesList[10];
	arretesList[10].p1 = &plane[R];
	arretesList[10].p2 = &plane[B];
	arretesList[10].name = "BR";

	arretesList[11].d = &ndcPoint[3];// left bot
	arretesList[11].f = &ndcPoint[7];
	plane[L].quatre=&arretesList[11];
	plane[B].quatre =&arretesList[11];
	arretesList[11].p1 = &plane[B];
	arretesList[11].p2 = &plane[L];
	arretesList[11].name = "LB";



	for(int i =0;i<12;i++)
	{
		vec3 dir = normalize(arretesList[i].f->pos-arretesList[i].d->pos); 
	
	}

}


Frustum::Frustum()
{
	GPlanes = new GPUBuffer("planes");
	GPlanes->create(41*sizeof(vec4),GL_SHADER_STORAGE_BUFFER ,GL_DYNAMIC_COPY);

}


void Frustum::getEdge(int indice, vec3 * v1, vec3 * v2)
{
	*v1 = arretesList[indice].d->pos;
	*v2 = arretesList[indice].f->pos;
}

void Frustum::computeConvex(vec3 l)
{
	int nbr = 0;

	bool isLightIn = true;
	for(int i = 0;i<6;i++)
	{

		if(!insidePlane2(plane[i].equation,l)) //on élimine si nécessaire les plans du frustrum
		{
			plane[i].un->cpt--;
			plane[i].deux->cpt--;
			plane[i].trois->cpt--;
			plane[i].quatre->cpt--;

			plane[i].un->dad = i;
			plane[i].deux->dad = i;
			plane[i].trois->dad = i;
			plane[i].quatre->dad = i;

			plane[i].toKeep = false;
			isLightIn = false;
		}
		else
		{
			convex[nbr] = plane[i].equation;
			nbr++;
		}
	}
	
	int tmpNbr = nbr;

	for(int i = 0;i<12;i++) //on crée de nouveau plan au besoin
	{
		if(arretesList[i].cpt==1)
		{
		    //bool p1 = insidePlane2(arretesList[i].p1->equation,l);	
		    //bool p2 = insidePlane2(arretesList[i].p2->equation,l);	
			//convex[nbr] =-normalize( cptPlane2(l,arretesList[i].d->pos, arretesList[i].f->pos) );
			vec3 normal = normalize( cross(arretesList[i].d->pos - l, arretesList[i].f->pos - l) );
			convex[nbr] = vec4( normal, -dot(normal, l) );
			//if(dot(plane[arretesList[i].dad].equation,vec4(convex[nbr].x,convex[nbr].y,convex[nbr].z,0))<0)
			if ( dot( convex[nbr], vec4(inside, 1) ) < 0.0 )
			 	convex[nbr]=-convex[nbr];
			nbr++;
		}
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, GPlanes->getBuffer());
	for(int i =0;i<6+nbr;i++)
	{
		vec4 toto=convex[i];
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, i*sizeof(vec4), sizeof(vec4), &toto);
	}
	
	int counter = 0;
	/*
	for(int i =6+nbr;i<6+nbr+8;i++)
	{
		if(!(!ndcPoint[i-6-nbr].p1->toKeep && !ndcPoint[i-6-nbr].p1->toKeep && !ndcPoint[i-6-nbr].p1->toKeep))
		{
		counter ++;
		vec4 toto=vec4(ndcPoint[i-6-nbr].pos,0);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, i*sizeof(vec4), sizeof(vec4), &toto);
		}
	}
*/
	// copie des 4 sommets du far en fin de tableau
		vec4 temp= vec4(ndcPoint[0].pos,0);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 36*sizeof(vec4), sizeof(vec4), &temp);	
		temp= vec4(ndcPoint[1].pos,0);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 37*sizeof(vec4), sizeof(vec4), &temp);	
		temp= vec4(ndcPoint[2].pos,0);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 38*sizeof(vec4), sizeof(vec4), &temp);	
		temp= vec4(ndcPoint[3].pos,0);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 39*sizeof(vec4), sizeof(vec4), &temp);	

		uvec4 tmp = uvec4(tmpNbr,tmpNbr,nbr,nbr+counter);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 40*sizeof(vec4), sizeof(uvec4), &tmp);	


	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	GPlanes->bind(40);
}
/*
void Frustum::draw(mat4 matrix,int n)
{
	

for(int i = 0;i<12;i++)
	{
		if(arretesList[i].cpt==2)
		{	
		glm::vec4 d  = matrix*glm::vec4(arretesList[i].d->pos,1);
		glm::vec4 f  = matrix*glm::vec4(arretesList[i].f->pos,1);
		glBegin(GL_POINTS);
		glColor3f(0,0,1);
		for(float a = 0.0;a<=1.0;a+=0.001)
			{
	    	glVertex3f(0.5*(a*d.x/d.w+(1.0-a)*f.x/f.w),0.5*(a* d.y/d.w+(1.0-a)*f.y/f.w),0.5*(a* d.z/d.w+(1.0-a)*f.z/f.w));
 	    	}
 	    glEnd();
 	    }
 	    if(arretesList[i].cpt==0)
 	    {
 	    glBegin(GL_POINTS);
		glColor3f(0,1,0);
		glm::vec4 d  = matrix*glm::vec4(arretesList[i].d->pos,1);
		glm::vec4 f  = matrix*glm::vec4(arretesList[i].f->pos,1);

		for(float a = 0.0;a<=1.0;a+=0.001)
			{
	    	glVertex3f(0.5*(a*d.x/d.w+(1.0-a)*f.x/f.w),0.5*(a* d.y/d.w+(1.0-a)*f.y/f.w),0.5*(a* d.z/d.w+(1.0-a)*f.z/f.w));
 	    	}
 	    glEnd();
	
 	    }
	}

glColor3f(1,0,0);	
glPointSize(3.0);
for(int i = 0;i<8;i++)
{
		glm::vec4 d  = matrix*glm::vec4(ndcPoint[i].pos,1);

		glBegin(GL_POINTS);
	    glVertex3f(0.5*d.x/d.w,0.5* d.y/d.w,0.5* d.z/d.w);
        glEnd();
}

}
*/