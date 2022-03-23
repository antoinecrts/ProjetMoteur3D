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

		string info_text; //stocke les logs retourn�s par opengl

		//Constructeur-Destructeur

		/**
			Constructeur de la classe GLProgram prenant en param�tre son nom et son type
			Permet de r�cup�rer le GLProgramSource correspondant au nom gr�ce � GLProgramSourceManager, cr�e un nouveau GLUniformManager et d�termine le type du programme
			@param name : nom du programme
			@param type : type du programme
		*/
		GLProgram(std::string name,GLenum type);

		/**
			Destructeur de la classe qui appelle la fonction release du GLProgramSourceManager
		*/
		~GLProgram();

		//M�thodes

		/**
			M�thode qui regarde si le GLProgramSource est existant et si il est valide en appelant sa m�thode isValid
			@return : bool�en qui retourne true si le GLProgram est existant et si il est valide
		*/
		bool isValid();

		/**
			M�thode retournant l'adresse m�moire allou� � l'instance du programme opengl
			@return : adresse m�moire du programme
		*/
		GLuint getProgram();

		/**
			M�thode retournant le type du programme (vs, fs, gs, etc ...)
			@return : type du programme
		*/
		GLenum getType();

		/**
			M�thode retournant le GLUniformManager
			@return : GLUniformManager
		*/
		GLUniformManager* uniforms();

		/**
			M�thode retournant le nom du programme suivi de son type (ex : vertex-VS)
			@return : nom du programme suivi de son type
		*/
		string getName();


		//STATIC

		static GLProgramsSourceManager prgMgr; //classe g�rant toutes les instances de GLProgramSource


	private:

		//Attributs

		string m_Name; // nom associ� � l'instance de GLProgram
		GLenum m_Type; //type du programme (vf, fs, gs, etc ...)
		GLProgramSource* src; //instance de GLProgramSource contenant le programme glsl 
		GLUniformManager *m_UniformMgr; //instance de GLUniformManager permettant de g�rer les variables uniforms du programme


};





#endif
