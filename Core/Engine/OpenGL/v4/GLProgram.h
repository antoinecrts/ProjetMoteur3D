/*
 *	(c) XLim, UMR-CNRS
 *	Authors: G.Gilet
 *  Documentation : J. Lemerle P. Le Gac
 */

#ifndef _GLPROGRAM_H
#define _GLPROGRAM_H

#include <GL/glew.h>
#include <string>
#include <stdexcept>

#include "Engine/OpenGL/Managers/GPUVariable.hpp"
#include "Engine/OpenGL/Managers/GLUniformManager.h"
#include "Engine/OpenGL/v4/GLProgramSource.h"
#include "Engine/OpenGL/Managers/GLProgramsSourceManager.h"

using namespace std;

class GLProgram
{
	public:

		//Attributs

		string info_text; //stocke les logs retournés par opengl

		//Constructeur-Destructeur

		/**
			Constructeur de la classe GLProgram prenant en paramètre son nom et son type
			Permet de récupérer le GLProgramSource correspondant au nom grâce à GLProgramSourceManager, crée un nouveau GLUniformManager et détermine le type du programme
			@param name : nom du programme
			@param type : type du programme
		*/
		GLProgram(std::string name,GLenum type);

		/**
			Destructeur de la classe qui appelle la fonction release du GLProgramSourceManager
		*/
		~GLProgram();

		//Méthodes

		/**
			Méthode qui regarde si le GLProgramSource est existant et si il est valide en appelant sa méthode isValid
			@return : booléen qui retourne true si le GLProgram est existant et si il est valide
		*/
		bool isValid();

		/**
			Méthode retournant l'adresse mémoire alloué à l'instance du programme opengl
			@return : adresse mémoire du programme
		*/
		GLuint getProgram();

		/**
			Méthode retournant le type du programme (vs, fs, gs, etc ...)
			@return : type du programme
		*/
		GLenum getType();

		/**
			Méthode retournant le GLUniformManager
			@return : GLUniformManager
		*/
		GLUniformManager* uniforms();

		/**
			Méthode retournant le nom du programme suivi de son type (ex : vertex-VS)
			@return : nom du programme suivi de son type
		*/
		string getName();


		//STATIC

		static GLProgramsSourceManager prgMgr; //classe gérant toutes les instances de GLProgramSource


	private:

		//Attributs

		string m_Name; // nom associé à l'instance de GLProgram
		GLenum m_Type; //type du programme (vf, fs, gs, etc ...)
		GLProgramSource* src; //instance de GLProgramSource contenant le programme glsl 
		GLUniformManager *m_UniformMgr; //instance de GLUniformManager permettant de gérer les variables uniforms du programme


};





#endif
