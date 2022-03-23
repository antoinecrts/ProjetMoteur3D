#pragma once

#include "Engine/OpenGL/MaterialGL.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Scene.h"

/**
	Classe repr�sentant un mat�riau import� depuis un fichier de configuration
	les shaders et param�tres (uniforms...) sont cr��s � la vol�e d'apr�s ce dernier
*/
class BRDFMaterial : public MaterialGL{
	public :

		//Constructeur - Destructeur

		/**
			Constructeur de la classe BRDFMaterial
			prend en param�tre les noms n�c�ssaire � la gestion du Material par le manager de GobLim
			@param name : premier nom
			@param className : second nom
		*/
		BRDFMaterial(string name, string className);

		/*
			Destructeur par d�faut de la classe BRDFMaterial
		*/
		~BRDFMaterial();

		//Getters, setters

		/**
			M�thode permettant de cr�er / modifier une variable uniform de type float d'apr�s son nom dans le shader sp�cifi�
			@param typeShader : le shader dans lequel est utilis� la variable uniform
			@param uniformName : le nom de la variable uniform
			@param value : la valeur de la variable � cr�er / modifier
		*/
		void setUniformFloat(GLenum typeShader, string uniformName, float value);

		/**
		M�thode permettant de cr�er / modifier une variable uniform de type vec4 d'apr�s son nom dans le shader sp�cifi�
		@param typeShader : le shader dans lequel est utilis� la variable uniform
		@param uniformName : le nom de la variable uniform
		@param value : la valeur de la variable � cr�er / modifier
		*/
		void setUniformVec4(GLenum typeShader, string uniformName, glm::vec4 &value);

		/**
			M�thode permettant de cr�er / modifier une variable uniform de type vec3 d'apr�s son nom dans le shader sp�cifi�
			@param typeShader : le shader dans lequel est utilis� la variable uniform
			@param uniformName : le nom de la variable uniform
			@param value : la valeur de la variable � cr�er / modifier
		*/
		void setUniformVec3(GLenum typeShader, string uniformName, glm::vec3 &value);

		/**
		M�thode permettant de cr�er / modifier une variable uniform de type bool d'apr�s son nom dans le shader sp�cifi�
		@param typeShader : le shader dans lequel est utilis� la variable uniform
		@param uniformName : le nom de la variable uniform
		@param value : la valeur de la variable � cr�er / modifier
		*/
		void setUniformBool(GLenum typeShader, string uniformName, bool value);

		/**
			M�thode permettant de r�cuperer la valeur d'une variable uniform de type float dans un shader sp�cifique
			@param typeShader : le shader dans lequel est utilis� la variable uniform
			@param : uniformName : le nom de la variable uniform dont on r�cup�re la valeur
		*/
		float getUniformFloat(GLenum typeShader, string uniformName);

		/**
			M�thode permettant de r�cuperer la valeur d'une variable uniform de type vec4 dans un shader sp�cifique
			@param typeShader : le shader dans lequel est utilis� la variable uniform
			@param : uniformName : le nom de la variable uniform dont on r�cup�re la valeur
		*/
		glm::vec4 getUniformVec4(GLenum typeShader, string uniformName);

		/**
			M�thode permettant de r�cuperer la valeur d'une variable uniform de type vec3 dans un shader sp�cifique
			@param typeShader : le shader dans lequel est utilis� la variable uniform
			@param : uniformName : le nom de la variable uniform dont on r�cup�re la valeur
		*/
		glm::vec3 getUniformVec3(GLenum typeShader, string uniformName);

		/**
		M�thode permettant de r�cuperer la valeur d'une variable uniform de type bool dans un shader sp�cifique
		@param typeShader : le shader dans lequel est utilis� la variable uniform
		@param : uniformName : le nom de la variable uniform dont on r�cup�re la valeur
		*/
		bool getUniformBool(GLenum typeShader, string uniformName);

		void addUniformFloatInList(string uniformName, GLenum typeShader, float value);
		void addUniformVec3InList(string uniformName, GLenum typeShader, glm::vec3 &value);
		void addUniformVec4InList(string uniformName, GLenum typeShader, glm::vec4 &value);
		void addUniformBoolInList(string uniformName, GLenum typeShader, bool value);
		bool isUniformExist(string type, string uniformName, GLenum typeShader);

		virtual void render(Node *o);
		virtual void update(Node* o, const int elapsedTime);

		virtual void displayInterface(const char* title);

	private :

		//M�thodes

		/**
			M�thode r�cup�rant le GLProgram g�rant le type de shader pass� en param�tre
			@param shaderType : le type de shader
			@return : le pointeur de l'instance du GLProgram associ�
		*/
		GLProgram* getProgramByType(GLenum shaderType);

		GPUmat4* modelViewProj;
		GPUmat4* modelView;
		GPUmat4* modelViewF;

		map<pair<string, GLenum>, float> listUniformFloat;
		map<pair<string, GLenum>, glm::vec3> listUniformVec3;
		map<pair<string, GLenum>, glm::vec4> listUniformVec4;
		map<pair<string, GLenum>, bool> listUniformBool;

};