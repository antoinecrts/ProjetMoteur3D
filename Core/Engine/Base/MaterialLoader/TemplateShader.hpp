/*
	(c) XLim, UMR-CNRS
	Authors: J.Lemerle
	Documentation : J. Lemerle P. Le Gac
*/

#pragma once

#include <map>
#include <string>

/**
	Classe permettant de faire de la complétion de template de shader glsl plus facilement au moyen de "flags" (notés ***FLAG***)
	placés aux endroits du template qui doivent être complété par du code glsl non statique (les includes, les variables uniforms / in / out,
	les données récupérées via les layouts d'opengl et le code de la fonction main)
	Les templates sont chargés / écrits dans des fichiers.
*/
class TemplateShader {

	

	public:

		//Constructeur - Destructeur

		/**
			Constructeur de la classe TemplateShader, qui va importer depuis un fichier le template de shader, et chercher la position
			des flags de code à remplir
			@param filename : le chemin d'accès au fichier du template
		*/
		TemplateShader(std::string filename);

		/*
			Destructeur de TemplateShader
		*/
		~TemplateShader();

		//Méthodes

		/**
			Méthode qui importe un template de shader depuis un fichier et mémorise la position des flags de code à remplir
			Réinitialise toutes les données déjà existantes dans l'instance de TemplateMaterial (template chargé, snippets de code ajoutés, etc...)
			@param filename : le chemin d'accès au fichier du template
			@return : indicateur si le template de fichier a bien été chargé
		*/
		bool readTemplate(std::string filename);


		/**
			Méthode qui va écrire le shader dans un fichier. Vérifie si le template a bien été complété et validé (tous les flags ont été remplacés
			par du code) et que la méthode validateShader() renvoie true
			@param filename : le chemin d'accès pour écrire dans le fichier du shader
			@return : indicateur si le template a bien été écrit dans le fichier
		*/
		bool writeTemplate(std::string filename);

		/**
			Méthode qui permet d'ajouter les lignes de codes dans la partie ***INCLUDES*** du template
			si le template n'a pas déjà été validé (les flags de code ont été remplacés et donc pas possible de réécrire à l'emplacement)
			@param content : les lignes de codes qui seront ajoutés à l'emplacement du flag ***INCLUDES***
			@return : indicateur si le contenu a bien été pris en compte
		*/
		bool setIncludes(std::string content);

		/**
		Méthode qui permet d'ajouter les lignes de codes dans la partie ***UNIFORMS*** du template
		si le template n'a pas déjà été validé (les flags de code ont été remplacés et donc pas possible de réécrire à l'emplacement)
		@param content : les lignes de codes qui seront ajoutés à l'emplacement du flag ***UNIFORMS***
		@return : indicateur si le contenu a bien été pris en compte
		*/
		bool setUniforms(std::string content);

		/**
		Méthode qui permet d'ajouter les lignes de codes dans la partie ***IN-OUT*** du template
		si le template n'a pas déjà été validé (les flags de code ont été remplacés et donc pas possible de réécrire à l'emplacement)
		@param content : les lignes de codes qui seront ajoutés à l'emplacement du flag ***IN-OUT***
		@return : indicateur si le contenu a bien été pris en compte
		*/
		bool setInOut(std::string content);

		/**
		Méthode qui permet d'ajouter les lignes de codes dans la partie ***LAYOUT*** du template
		si le template n'a pas déjà été validé (les flags de code ont été remplacés et donc pas possible de réécrire à l'emplacement)
		@param content : les lignes de codes qui seront ajoutés à l'emplacement du flag ***LAYOUT***
		@return : indicateur si le contenu a bien été pris en compte
		*/
		bool setLayout(std::string content);

		/**
		Méthode qui permet d'ajouter les lignes de codes dans la partie ***MAIN-CODE*** du template
		si le template n'a pas déjà été validé (les flags de code ont été remplacés et donc pas possible de réécrire à l'emplacement)
		@param content : les lignes de codes qui seront ajoutés à l'emplacement du flag ***MAIN-CODE***
		@return : indicateur si le contenu a bien été pris en compte
		*/
		bool setMainCode(std::string content);

		/**
			Méthode qui va permettre l'écriture des snippets de code glsl dans le template du shader si cela n'a pas déjà été fait
			ou indique l'état de validation du shader
			@return Renvoie vrai si le shader a déjà été validé ou vient de l'être (ajout des snippets), ou false si des snippets sont manquants
			pour compléter le shader
		*/
		bool validateShader();

	private:

		//Attributs

		std::string * shaderSource; // code du template chargé par l'instance de classe depuis un fichier

		std::string * includes; // snippet de code à mettre à l'emplacement ***INCLUDES***
		std::string * uniforms; // snippet de code à mettre à l'emplacement ***UNIFORMS***
		std::string * inOut; // snippet de code à mettre à l'emplacement ***IN-OUT***
		std::string * layout; // snippet de code à mettre à l'emplacement ***LAYOUT***
		std::string * mainCode; // snippet de code à mettre à l'emplacement ***MAIN-CODE***

		bool isValidated; //statut indiquant si les snippets ont bien été inclus dans le template de code

		// Méthodes

		/*
			Méthode qui supprime tous les attributs (buffers pour snippets et template de code, positions des flags...)
			de l'instance de classe
		*/
		void deleteAttributes();
};