#pragma once

#include "Engine/OpenGL/MaterialGL.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Scene.h"

/**
	Classe représentant un matériau importé depuis un fichier de configuration
	les shaders et paramètres (uniforms...) sont créés à la volée d'après ce dernier
*/
class BRDFMaterial : public MaterialGL{
	public :

		//Constructeur - Destructeur

		/**
			Constructeur de la classe BRDFMaterial
			prend en paramètre les noms nécéssaire à la gestion du Material par le manager de GobLim
			@param name : premier nom
			@param className : second nom
		*/
		BRDFMaterial(string name, string className);

		/*
			Destructeur par défaut de la classe BRDFMaterial
		*/
		~BRDFMaterial();

		//Getters, setters

		/**
			Méthode permettant de créer / modifier une variable uniform de type float d'après son nom dans le shader spécifié
			@param typeShader : le shader dans lequel est utilisé la variable uniform
			@param uniformName : le nom de la variable uniform
			@param value : la valeur de la variable à créer / modifier
		*/
		void setUniformFloat(GLenum typeShader, string uniformName, float value);

		/**
		Méthode permettant de créer / modifier une variable uniform de type vec4 d'après son nom dans le shader spécifié
		@param typeShader : le shader dans lequel est utilisé la variable uniform
		@param uniformName : le nom de la variable uniform
		@param value : la valeur de la variable à créer / modifier
		*/
		void setUniformVec4(GLenum typeShader, string uniformName, glm::vec4 &value);

		/**
			Méthode permettant de créer / modifier une variable uniform de type vec3 d'après son nom dans le shader spécifié
			@param typeShader : le shader dans lequel est utilisé la variable uniform
			@param uniformName : le nom de la variable uniform
			@param value : la valeur de la variable à créer / modifier
		*/
		void setUniformVec3(GLenum typeShader, string uniformName, glm::vec3 &value);

		/**
		Méthode permettant de créer / modifier une variable uniform de type bool d'après son nom dans le shader spécifié
		@param typeShader : le shader dans lequel est utilisé la variable uniform
		@param uniformName : le nom de la variable uniform
		@param value : la valeur de la variable à créer / modifier
		*/
		void setUniformBool(GLenum typeShader, string uniformName, bool value);

		/**
			Méthode permettant de récuperer la valeur d'une variable uniform de type float dans un shader spécifique
			@param typeShader : le shader dans lequel est utilisé la variable uniform
			@param : uniformName : le nom de la variable uniform dont on récupère la valeur
		*/
		float getUniformFloat(GLenum typeShader, string uniformName);

		/**
			Méthode permettant de récuperer la valeur d'une variable uniform de type vec4 dans un shader spécifique
			@param typeShader : le shader dans lequel est utilisé la variable uniform
			@param : uniformName : le nom de la variable uniform dont on récupère la valeur
		*/
		glm::vec4 getUniformVec4(GLenum typeShader, string uniformName);

		/**
			Méthode permettant de récuperer la valeur d'une variable uniform de type vec3 dans un shader spécifique
			@param typeShader : le shader dans lequel est utilisé la variable uniform
			@param : uniformName : le nom de la variable uniform dont on récupère la valeur
		*/
		glm::vec3 getUniformVec3(GLenum typeShader, string uniformName);

		/**
		Méthode permettant de récuperer la valeur d'une variable uniform de type bool dans un shader spécifique
		@param typeShader : le shader dans lequel est utilisé la variable uniform
		@param : uniformName : le nom de la variable uniform dont on récupère la valeur
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

		//Méthodes

		/**
			Méthode récupérant le GLProgram gérant le type de shader passé en paramètre
			@param shaderType : le type de shader
			@return : le pointeur de l'instance du GLProgram associé
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