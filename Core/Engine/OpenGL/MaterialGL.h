/*
 *	(c) XLim, UMR-CNRS
 *	Authors: G.Gilet
 *  Documentation : J. Lemerle P. Le Gac
 */

#ifndef _MATERIALGL_
#define _MATERIALGL_

#include "Engine/Base/Material.h"
#include "Engine/Base/Resource.h"
#include "Utils/Resource_mgr.hpp"
#include "Engine/OpenGL/v4/GLProgramPipeline.h"




class MaterialGL : public Material
{
	public :

		//Attributs

		GLProgramPipeline* m_ProgramPipeline;
		
		//Constructeur-Destructeur

		/**
			Constructeur de la classe � partir du nom du mat�riau et du nom de la classe
			@param name : nom du mat�riau
			@param className : nom de la classe
		*/
		MaterialGL(string name,string className);

		/**
			Destructeur de la classe
		*/
		~MaterialGL();

		//M�thodes

		/**
			M�thode virtuelle qui est appel�e pour faire le rendu d'un objet en utilisant ce mat�riau
			@param o : Node/Objet pour lequel on veut effectuer le rendu
		*/
		virtual void render(Node *o);

		/**
			M�thode virtuelle qui est appel�e pour modifier une valeur d'un param�tre n�cessaire pour le rendu
			@param o : Node/Objet concern� par le rendu
			@param elapsedTime : temps
		*/
		virtual void update(Node *o, const int elapsedTime){}

		
	protected :
		
		//Attributs

		// Vertex / Geometry / Tessel Control / Tessel Eval / Fragment
		GLProgram *vp, *gp, *tcp, *tep, *fp;
};

#endif