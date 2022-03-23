/*
 *	(c) XLim, UMR-CNRS
 *	Authors: G.Gilet
 *  Documentation : J. Lemerle P. Le Gac
 */

#ifndef _GLPROGRAMSOURCE_H
#define _GLPROGRAMSOURCE_H

#include <GL/glew.h>
#include <string>
#include <stdexcept>

#include "Engine/OpenGL/Managers/GPUVariable.hpp"
#include "Engine/OpenGL/Managers/GLUniformManager.h"

using namespace std;

/**
* Classe s'occupant de cr�er les shaders objects dans opengl � partir d'un code source en GLSL
*/
class GLProgramSource
{
	public:

		//Attributs

		string info_text; //stockage des logs du program pipeline object et du shader object d'opengl associ�s au shader

		bool loaded; //indique si le programme a bien charg� le shader en exec dans opengl

		//Constructeur - Destructeur

		/**
		* Constructeur de la classe GLProgramSource, qui prend en param�tre un nom de programme
		* initialise variable d'adressage m�moire dans opengl, son statut
		* @param name : le nom du programme
		*/
		GLProgramSource(string name);

		/**
		* Destructeur de la classe GLProgramSource, qui appelle la fonction glDeleteProgram d'OpenGL
		*/
		~GLProgramSource();

		//M�thodes

		/**
		* M�thode qui permet de modifier le chemin d'acc�s vers le fichier contenant le shader de GLProgramSource
		* @param pathfilename : le chemin d'acc�s du fichier du shader
		*/
		void setFilename(string pathfilename);

		/**
		* M�thode qui cr�e le shader au sein d'opengl d'apr�s un type de shader sp�cifi� et le chemin d'acc�s du fichier du shader
		* @param shaderType : le type de shader qui doit �tre cr�� dans opengl (voir documentation pour connaitre correspondances)
		* @param filename : le chemin d'acc�s du fichier depuis lequel est import� le contenu du shader
		* @param return :
		*/
		bool createProgram(GLenum shaderType, std::string filename);

		/**
		* M�thode qui regarde le statut du program m_Program qui g�re le shader compil� dans opengl et v�rifie si celui ci est bien charg� / valide
		* @return : bool�en qui renvoie true si le programme est valid�
		*/
		bool isValid();

		/**
		* M�thode qui retourne l'adresse m�moire allou� � l'instance du program d'opengl qui g�re le shader compil�
		* @return : l'adresse m�moire du programme dans opengl
		*/
		GLuint getProgram();

		/**
		* M�thode qui r�cup�re les logs concernant le program qui link le shader compil� depuis opengl
		* ajoute les informations r�cup�r�es dans info_text
		*/
		void printProgramInfoLog();

		/**
		* M�thode qui va r�cup�rer et stocker r�cursivement les diff�rents codes sources n�c�ssaires � la cr�ation du shader dans opengl
		* @param analysedSource : code source de shader dans lequel on recherche r�cursivement les includes
		* @param ptrListOfSources : la liste des codes sources (shader de base inclu)
		* @param prtListOfLength : la liste stockant la tailles des string des codes sources r�cup�r�s
		*/
		void parseSources(string analysedSource, vector<const char*>* ptrlistOfSources, vector<int>* ptrlistOfLength);

		//STATIC

		// Gestion des named string en static

		static bool noIncludes; //variable statique qui indique si un shader a �t� import� depuis un fichier
		static map<string, const char*> m_IncludeSource; //map statique dans lequel sont stock�s les shaders import�s depuis un fichier et identifi�s par un nom

		/**
		* Fonction statique qui charge un programme shader d'apr�s le nom du fichier pass� en param�tre
		* et est stock� dans un map statique de GLProgramSource d'apr�s sa cl� @name
		* @param name : le nom qui identifie le programme shader dans le map
		* @param filename : le fichier depuis lequel on importe le programme shader
		*/
		static void createNamedString(string name, string filename);

		/**
		* Fonction statique qui va supprimer un programme shader du map de GLProgramSource d'apr�s sa cl� @name
		* @param name : le nom qui identifie le programme shader dans le map de GLProgramSource
		*/
		static void deleteNamedString(string name);

	private:

		//Attributs

		GLenum shaderType; //Type de shader charg� dans opengl (voir documentation enum shaderType d'opengl pour correspondance)

		string shaderSource; // code source du shader de base
		vector<string> listOfIncludeFiles; //[NON UTILISE]
		vector<const char*> listOfSources; //liste des diff�rents codes sources (includes) utilis�s par le shader
		vector<int>	listOfLength; // liste des tailles des codes sources utilis�s par le shader

		string m_Filename; //Chemin d'acc�s au fichier stockant le code source du shader
		GLuint m_Program; //Adresse de r�f�rence du programme charg� dans opengl (renvoy�e par glCreateProgram)

		//M�thodes

		/**
		* M�thode appel�e si il y a des erreurs � la cr�ation du shader dans opengl. R�cup�re le contenu de info_text �galement
		* et renvoie le tout sous forme de string
		* @return : les informations d'info_text et indique une erreur � la cr�ation du shader (type, nom du fichier, etc..)
		*/
		string printErrorString();

		/**
		* M�thode qui r�cup�re les logs concernant le shader depuis opengl
		* ajoute les informations r�cup�r�es dans info_text
		*/
		void printShaderInfoLog(GLuint shader);
};





#endif
