/*
	(c) XLim, UMR-CNRS
	Authors: J.Lemerle
	Documentation : J. Lemerle P. Le Gac
*/

#pragma once

#include <map>
#include <string>

/**
	Classe permettant de faire de la compl�tion de template de shader glsl plus facilement au moyen de "flags" (not�s ***FLAG***)
	plac�s aux endroits du template qui doivent �tre compl�t� par du code glsl non statique (les includes, les variables uniforms / in / out,
	les donn�es r�cup�r�es via les layouts d'opengl et le code de la fonction main)
	Les templates sont charg�s / �crits dans des fichiers.
*/
class TemplateShader {

	

	public:

		//Constructeur - Destructeur

		/**
			Constructeur de la classe TemplateShader, qui va importer depuis un fichier le template de shader, et chercher la position
			des flags de code � remplir
			@param filename : le chemin d'acc�s au fichier du template
		*/
		TemplateShader(std::string filename);

		/*
			Destructeur de TemplateShader
		*/
		~TemplateShader();

		//M�thodes

		/**
			M�thode qui importe un template de shader depuis un fichier et m�morise la position des flags de code � remplir
			R�initialise toutes les donn�es d�j� existantes dans l'instance de TemplateMaterial (template charg�, snippets de code ajout�s, etc...)
			@param filename : le chemin d'acc�s au fichier du template
			@return : indicateur si le template de fichier a bien �t� charg�
		*/
		bool readTemplate(std::string filename);


		/**
			M�thode qui va �crire le shader dans un fichier. V�rifie si le template a bien �t� compl�t� et valid� (tous les flags ont �t� remplac�s
			par du code) et que la m�thode validateShader() renvoie true
			@param filename : le chemin d'acc�s pour �crire dans le fichier du shader
			@return : indicateur si le template a bien �t� �crit dans le fichier
		*/
		bool writeTemplate(std::string filename);

		/**
			M�thode qui permet d'ajouter les lignes de codes dans la partie ***INCLUDES*** du template
			si le template n'a pas d�j� �t� valid� (les flags de code ont �t� remplac�s et donc pas possible de r��crire � l'emplacement)
			@param content : les lignes de codes qui seront ajout�s � l'emplacement du flag ***INCLUDES***
			@return : indicateur si le contenu a bien �t� pris en compte
		*/
		bool setIncludes(std::string content);

		/**
		M�thode qui permet d'ajouter les lignes de codes dans la partie ***UNIFORMS*** du template
		si le template n'a pas d�j� �t� valid� (les flags de code ont �t� remplac�s et donc pas possible de r��crire � l'emplacement)
		@param content : les lignes de codes qui seront ajout�s � l'emplacement du flag ***UNIFORMS***
		@return : indicateur si le contenu a bien �t� pris en compte
		*/
		bool setUniforms(std::string content);

		/**
		M�thode qui permet d'ajouter les lignes de codes dans la partie ***IN-OUT*** du template
		si le template n'a pas d�j� �t� valid� (les flags de code ont �t� remplac�s et donc pas possible de r��crire � l'emplacement)
		@param content : les lignes de codes qui seront ajout�s � l'emplacement du flag ***IN-OUT***
		@return : indicateur si le contenu a bien �t� pris en compte
		*/
		bool setInOut(std::string content);

		/**
		M�thode qui permet d'ajouter les lignes de codes dans la partie ***LAYOUT*** du template
		si le template n'a pas d�j� �t� valid� (les flags de code ont �t� remplac�s et donc pas possible de r��crire � l'emplacement)
		@param content : les lignes de codes qui seront ajout�s � l'emplacement du flag ***LAYOUT***
		@return : indicateur si le contenu a bien �t� pris en compte
		*/
		bool setLayout(std::string content);

		/**
		M�thode qui permet d'ajouter les lignes de codes dans la partie ***MAIN-CODE*** du template
		si le template n'a pas d�j� �t� valid� (les flags de code ont �t� remplac�s et donc pas possible de r��crire � l'emplacement)
		@param content : les lignes de codes qui seront ajout�s � l'emplacement du flag ***MAIN-CODE***
		@return : indicateur si le contenu a bien �t� pris en compte
		*/
		bool setMainCode(std::string content);

		/**
			M�thode qui va permettre l'�criture des snippets de code glsl dans le template du shader si cela n'a pas d�j� �t� fait
			ou indique l'�tat de validation du shader
			@return Renvoie vrai si le shader a d�j� �t� valid� ou vient de l'�tre (ajout des snippets), ou false si des snippets sont manquants
			pour compl�ter le shader
		*/
		bool validateShader();

	private:

		//Attributs

		std::string * shaderSource; // code du template charg� par l'instance de classe depuis un fichier

		std::string * includes; // snippet de code � mettre � l'emplacement ***INCLUDES***
		std::string * uniforms; // snippet de code � mettre � l'emplacement ***UNIFORMS***
		std::string * inOut; // snippet de code � mettre � l'emplacement ***IN-OUT***
		std::string * layout; // snippet de code � mettre � l'emplacement ***LAYOUT***
		std::string * mainCode; // snippet de code � mettre � l'emplacement ***MAIN-CODE***

		bool isValidated; //statut indiquant si les snippets ont bien �t� inclus dans le template de code

		// M�thodes

		/*
			M�thode qui supprime tous les attributs (buffers pour snippets et template de code, positions des flags...)
			de l'instance de classe
		*/
		void deleteAttributes();
};