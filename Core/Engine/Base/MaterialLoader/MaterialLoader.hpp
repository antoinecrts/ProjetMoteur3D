/*
	(c) XLim, UMR-CNRS
	Authors: J.Lemerle
	Documentation : J. Lemerle P. Le Gac
*/

#pragma once

#include "Materials/BRDFMaterial/BRDFMaterial.hpp"
#include "Utils/json-develop/src/json.hpp"
#include "Utils/Constants.h"

#include <string>

/**
	Classe définissant les fonctions pour importer des Material depuis des fichiers de configuration
*/
class MaterialLoader {
	public :
		/**
			Fonction qui créée une instance de BRDFMaterial et les shaders associés d'après un fichier de configuration passé en JSON
			@param filename : le chemin d'accès au fichier de configuration
			@return : un pointeur vers l'instance de BRDFMaterial créée
		*/
		static BRDFMaterial* loadMaterialFromJSON(std::string filename);
};