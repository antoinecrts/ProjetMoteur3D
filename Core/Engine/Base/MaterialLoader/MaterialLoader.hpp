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
	Classe d�finissant les fonctions pour importer des Material depuis des fichiers de configuration
*/
class MaterialLoader {
	public :
		/**
			Fonction qui cr��e une instance de BRDFMaterial et les shaders associ�s d'apr�s un fichier de configuration pass� en JSON
			@param filename : le chemin d'acc�s au fichier de configuration
			@return : un pointeur vers l'instance de BRDFMaterial cr��e
		*/
		static BRDFMaterial* loadMaterialFromJSON(std::string filename);
};