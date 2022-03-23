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
* Classe s'occupant de créer les shaders objects dans opengl à partir d'un code source en GLSL
*/
class GLProgramSource
{
	public:

		//Attributs

		string info_text; //stockage des logs du program pipeline object et du shader object d'opengl associés au shader

		bool loaded; //indique si le programme a bien chargé le shader en exec dans opengl

		//Constructeur - Destructeur

		/**
		* Constructeur de la classe GLProgramSource, qui prend en paramètre un nom de programme
		* initialise variable d'adressage mémoire dans opengl, son statut
		* @param name : le nom du programme
		*/
		GLProgramSource(string name);

		/**
		* Destructeur de la classe GLProgramSource, qui appelle la fonction glDeleteProgram d'OpenGL
		*/
		~GLProgramSource();

		//Méthodes

		/**
		* Méthode qui permet de modifier le chemin d'accès vers le fichier contenant le shader de GLProgramSource
		* @param pathfilename : le chemin d'accès du fichier du shader
		*/
		void setFilename(string pathfilename);

		/**
		* Méthode qui crée le shader au sein d'opengl d'après un type de shader spécifié et le chemin d'accès du fichier du shader
		* @param shaderType : le type de shader qui doit être créé dans opengl (voir documentation pour connaitre correspondances)
		* @param filename : le chemin d'accès du fichier depuis lequel est importé le contenu du shader
		* @param return :
		*/
		bool createProgram(GLenum shaderType, std::string filename);

		/**
		* Méthode qui regarde le statut du program m_Program qui gère le shader compilé dans opengl et vérifie si celui ci est bien chargé / valide
		* @return : booléen qui renvoie true si le programme est validé
		*/
		bool isValid();

		/**
		* Méthode qui retourne l'adresse mémoire alloué à l'instance du program d'opengl qui gère le shader compilé
		* @return : l'adresse mémoire du programme dans opengl
		*/
		GLuint getProgram();

		/**
		* Méthode qui récupère les logs concernant le program qui link le shader compilé depuis opengl
		* ajoute les informations récupérées dans info_text
		*/
		void printProgramInfoLog();

		/**
		* Méthode qui va récupérer et stocker récursivement les différents codes sources nécéssaires à la création du shader dans opengl
		* @param analysedSource : code source de shader dans lequel on recherche récursivement les includes
		* @param ptrListOfSources : la liste des codes sources (shader de base inclu)
		* @param prtListOfLength : la liste stockant la tailles des string des codes sources récupérés
		*/
		void parseSources(string analysedSource, vector<const char*>* ptrlistOfSources, vector<int>* ptrlistOfLength);

		//STATIC

		// Gestion des named string en static

		static bool noIncludes; //variable statique qui indique si un shader a été importé depuis un fichier
		static map<string, const char*> m_IncludeSource; //map statique dans lequel sont stockés les shaders importés depuis un fichier et identifiés par un nom

		/**
		* Fonction statique qui charge un programme shader d'après le nom du fichier passé en paramètre
		* et est stocké dans un map statique de GLProgramSource d'après sa clé @name
		* @param name : le nom qui identifie le programme shader dans le map
		* @param filename : le fichier depuis lequel on importe le programme shader
		*/
		static void createNamedString(string name, string filename);

		/**
		* Fonction statique qui va supprimer un programme shader du map de GLProgramSource d'après sa clé @name
		* @param name : le nom qui identifie le programme shader dans le map de GLProgramSource
		*/
		static void deleteNamedString(string name);

	private:

		//Attributs

		GLenum shaderType; //Type de shader chargé dans opengl (voir documentation enum shaderType d'opengl pour correspondance)

		string shaderSource; // code source du shader de base
		vector<string> listOfIncludeFiles; //[NON UTILISE]
		vector<const char*> listOfSources; //liste des différents codes sources (includes) utilisés par le shader
		vector<int>	listOfLength; // liste des tailles des codes sources utilisés par le shader

		string m_Filename; //Chemin d'accès au fichier stockant le code source du shader
		GLuint m_Program; //Adresse de référence du programme chargé dans opengl (renvoyée par glCreateProgram)

		//Méthodes

		/**
		* Méthode appelée si il y a des erreurs à la création du shader dans opengl. Récupère le contenu de info_text également
		* et renvoie le tout sous forme de string
		* @return : les informations d'info_text et indique une erreur à la création du shader (type, nom du fichier, etc..)
		*/
		string printErrorString();

		/**
		* Méthode qui récupère les logs concernant le shader depuis opengl
		* ajoute les informations récupérées dans info_text
		*/
		void printShaderInfoLog(GLuint shader);
};





#endif
