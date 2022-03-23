/*
	(c) XLim, UMR-CNRS
	Authors: J.Lemerle
	Documentation : J. Lemerle P. Le Gac
*/

#include "TemplateShader.hpp"

#include "Engine/OpenGL/Managers/textfile.h"

#include <iostream>

using namespace std;

TemplateShader::TemplateShader(std::string filename) {
	shaderSource = includes = uniforms = inOut = layout = mainCode = nullptr;

	if (!readTemplate(filename))
		throw "Fichier " + filename + "est introuvable ou a eu un problème d'import \n";
}

TemplateShader::~TemplateShader() {
	deleteAttributes();
}

bool TemplateShader::readTemplate(std::string filename){

	//chargement du fichier
	char * content_file = textFileRead((char*)filename.c_str());
	
	//vérification si le fichier a bien été chargé ou non (renvoie false directement dans le second cas
	if (content_file == NULL)
		return false;
	else {
		//Si un template a déjà été chargé, on nettoie les attributs alloués
		if(shaderSource != nullptr)
			deleteAttributes();

		//initialisation de l'attribut contenant le template importé du fichier
		//et du statut
		shaderSource = new string(content_file);
		isValidated = false;

		string flag; // flag à rechercher dans le template
		int position; //position du flag à récupérer depuis le début du string contenant le template

		// position depuis laquelle on commence à chercher le flag dans le template
		// fait commencer la recherche du prochain flag depuis la position de fin du précédent
		// dans le template
		int offset = 0; 

		flag = "***INCLUDES***"; // Flag du snippet pour les includes nécéssaires au shader
		position = shaderSource->find(flag, offset);
		if (shaderSource->begin() + position == shaderSource->end()) { // si flag non trouvé, renvoie false et affiche un message d'erreur
			cerr << "[Erreur] : le flag " << flag << "n'a pas été trouvé dans le template du fichier " << filename <<  endl;
			return false;
		}

		offset = position + flag.size(); 
		flag = "***UNIFORMS***"; // Flag du snippet pour les variables uniforms passées depuis le matériau au shader
		position = shaderSource->find(flag, offset);
		if (shaderSource->begin() + position == shaderSource->end()) {
			cerr << "[Erreur] : le flag " << flag << "n'a pas été trouvé dans le template " << endl;
			return false;
		}

		offset = position + flag.size();
		flag = "***IN-OUT***"; // Flag du snippet pour les variables in / out du shader
		position = shaderSource->find(flag, offset);
		if (shaderSource->begin() + position == shaderSource->end()) {
			cerr << "[Erreur] : le flag " << flag << "n'a pas été trouvé dans le template " << endl;
			return false;
		}

		offset = position + flag.size();
		flag = "***LAYOUT***"; // Flag du snippet pour les variables récupérées depuis le layout d'opengl
		position = shaderSource->find(flag, offset);
		if (shaderSource->begin() + position == shaderSource->end()) {
			cerr << "[Erreur] : le flag " << flag << "n'a pas été trouvé dans le template " << endl;
			return false;
		}

		offset = position + flag.size();
		flag = "***MAIN-CODE***"; // Flag du snippet pour le contenu de la fonction main
		position = shaderSource->find(flag, offset);
		if (shaderSource->begin() + position == shaderSource->end()) {
			cerr << "[Erreur] : le flag " << flag << "n'a pas été trouvé dans le template " << endl;
			return false;
		}

		//Si tout s'est déroulé sans problème, les infos sur les flags ont été récupérées, on renvoie true
		return true;
	}
}

bool TemplateShader::writeTemplate(std::string filename) {
	if (validateShader()) {
		cout << "write status " << textFileWrite((char*)filename.c_str(), (char*)shaderSource->c_str()) << endl;
		return true;
	}
	return false;
}

bool TemplateShader::setIncludes(std::string content){
	if (isValidated) {
		cerr << "Le template a déjà été validé, impossible de modifier le code attrayant aux includes." << endl;
		return false;
	}
	if (includes == nullptr)
		includes = new string();
	*includes = content;
	return true;
}

bool TemplateShader::setUniforms(std::string content){
	if (isValidated) {
		cerr << "Le template a déjà été validé, impossible de modifier le code attrayant aux variables uniforms." << endl;
		return false;
	}
	if (uniforms == nullptr)
		uniforms = new string();
	*uniforms = content;
	return true;
}

bool TemplateShader::setInOut(std::string content){
	if (isValidated) {
		cerr << "Le template a déjà été validé, impossible de modifier le code attrayant aux variables in / out." << endl;
		return false;
	}
	if (inOut == nullptr)
		inOut = new string();
	*inOut = content;
	return true;
}

bool TemplateShader::setLayout(std::string content){
	if (isValidated) {
		cerr << "Le template a déjà été validé, impossible de modifier le code attrayant aux variables du layout." << endl;
		return false;
	}
	if (layout == nullptr)
		layout = new string();
	*layout = content;
	return true;
}

bool TemplateShader::setMainCode(std::string content){
	if (isValidated) {
		cerr << "Le template a déjà été validé, impossible de modifier le code attrayant au code de la fonction main." << endl;
		return false;
	}
	if (mainCode == nullptr)
		mainCode = new string();
	*mainCode = content;
	return true;
}

bool TemplateShader::validateShader(){
	//Si le template a déjà été complété et validé, on retourne true
	if (isValidated == true)
		return isValidated;

	//Sinon..
	//Vérification que toutes les variables associés aux flags ont été initialisées (dans le cas contraire on renvoie false)
	if (includes == nullptr || uniforms == nullptr || inOut == nullptr || layout == nullptr || mainCode == nullptr) {
		cerr << "Un ou plusieurs des flags du template n'ont pas été complétés. " << endl;
		return false;
	}

	string flag;

	//Remplacement du flag ***INCLUDES***
	flag = "***INCLUDES***";
	shaderSource->replace(shaderSource->find(flag), flag.size(), *includes);

	//Remplacement du flag ***UNIFORMS***
	flag = "***UNIFORMS***";
	shaderSource->replace(shaderSource->find(flag), flag.size(), *uniforms);

	//Remplacement du flag ***IN-OUT***
	flag = "***IN-OUT***";
	shaderSource->replace(shaderSource->find(flag), flag.size(), *inOut);

	//Remplacement du flag ***LAYOUT***
	flag = "***LAYOUT***";
	shaderSource->replace(shaderSource->find(flag), flag.size(), *layout);

	//Remplacement du flag ***MAIN-CODE***
	flag = "***MAIN-CODE***";
	shaderSource->replace(shaderSource->find(flag), flag.size(), *mainCode);

	//Mise à jour du statut du template comme validé et le retourne
	isValidated = true;
	return isValidated;
}

void TemplateShader::deleteAttributes() {
	delete(shaderSource);
	delete(includes);
	delete(uniforms);
	delete(inOut);
	delete(layout);
	delete(mainCode);
}