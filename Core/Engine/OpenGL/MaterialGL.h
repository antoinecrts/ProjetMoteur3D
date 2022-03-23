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
			Constructeur de la classe à partir du nom du matériau et du nom de la classe
			@param name : nom du matériau
			@param className : nom de la classe
		*/
		MaterialGL(string name,string className);

		/**
			Destructeur de la classe
		*/
		~MaterialGL();

		//Méthodes

		/**
			Méthode virtuelle qui est appelée pour faire le rendu d'un objet en utilisant ce matériau
			@param o : Node/Objet pour lequel on veut effectuer le rendu
		*/
		virtual void render(Node *o);

		/**
			Méthode virtuelle qui est appelée pour modifier une valeur d'un paramètre nécessaire pour le rendu
			@param o : Node/Objet concerné par le rendu
			@param elapsedTime : temps
		*/
		virtual void update(Node *o, const int elapsedTime){}

		
	protected :
		
		//Attributs

		// Vertex / Geometry / Tessel Control / Tessel Eval / Fragment
		GLProgram *vp, *gp, *tcp, *tep, *fp;
};

#endif