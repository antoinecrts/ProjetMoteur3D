#include "BRDFMaterial.hpp"

using namespace std;

BRDFMaterial::BRDFMaterial(string name, string className) : MaterialGL(name, className) {

	modelViewProj = vp->uniforms()->getGPUmat4("MVP");
	modelView = vp->uniforms()->getGPUmat4("MV");

	modelViewF = vp->uniforms()->getGPUmat4("NormalMV");

}

BRDFMaterial::~BRDFMaterial() {

}

void BRDFMaterial::setUniformFloat(GLenum typeShader, string uniformName, float value) {
	getProgramByType(typeShader)->uniforms()->getGPUfloat(uniformName)->Set(value);
	addUniformFloatInList(uniformName, typeShader, value);
}

float BRDFMaterial::getUniformFloat(GLenum typeShader, string uniformName) { return getProgramByType(typeShader)->uniforms()->getGPUfloat(uniformName)->getValue(); }

void BRDFMaterial::setUniformBool(GLenum typeShader, string uniformName, bool value) {
	getProgramByType(typeShader)->uniforms()->getGPUbool(uniformName)->Set(value);
	addUniformBoolInList(uniformName, typeShader, value);
}

bool BRDFMaterial::getUniformBool(GLenum typeShader, string uniformName) { return getProgramByType(typeShader)->uniforms()->getGPUbool(uniformName)->getValue(); }


void BRDFMaterial::setUniformVec4(GLenum typeShader, string uniformName, glm::vec4 &value) {
	getProgramByType(typeShader)->uniforms()->getGPUvec4(uniformName)->Set(value);
	addUniformVec4InList(uniformName, typeShader, value);
}

glm::vec4 BRDFMaterial::getUniformVec4(GLenum typeShader, string uniformName) { return getProgramByType(typeShader)->uniforms()->getGPUvec4(uniformName)->getValue(); }

void BRDFMaterial::setUniformVec3(GLenum typeShader, string uniformName, glm::vec3 &value) {
	getProgramByType(typeShader)->uniforms()->getGPUvec3(uniformName)->Set(value);
	addUniformVec3InList(uniformName, typeShader, value);
}

glm::vec3 BRDFMaterial::getUniformVec3(GLenum typeShader, string uniformName) { return getProgramByType(typeShader)->uniforms()->getGPUvec3(uniformName)->getValue(); }

GLProgram* BRDFMaterial::getProgramByType(GLenum shaderType) {
	switch (shaderType) {
	case GL_VERTEX_SHADER:
		return vp;
	case GL_FRAGMENT_SHADER:
		return fp;
	default:
		throw "Erreur : type de shader inexistant ou incorrect";
	}
}

void BRDFMaterial::render(Node *o)
{

	if (m_ProgramPipeline)
	{
		m_ProgramPipeline->bind();

		modelViewProj->Set(o->frame()->getTransformMatrix());
		modelView->Set(o->frame()->getRootMatrix());
		modelViewF->Set(glm::transpose(glm::inverse(o->frame()->getRootMatrix())));

		o->drawGeometry(GL_TRIANGLES);
		m_ProgramPipeline->release();
	}
}

void BRDFMaterial::update(Node* o, const int elapsedTime)
{

	if (o->frame()->updateNeeded())
	{
		
	}

}

void BRDFMaterial::displayInterface(const char *title) {

	ImGui::Text("%s",title);
	ImGui::Separator();

	//On parcourt chacune de nos listes d'uniformes afin d'ajouter les valeurs a l'affichage
	for (auto it = listUniformFloat.begin(); it != listUniformFloat.end(); ++it) {

		//On recupere le nom de l'uniforme
		pair<string, GLenum> uniform = it->first;
		string uniformName = uniform.first;

		//On recupere la valeur dans la liste
		float value = it->second;

		char* name = new char[uniformName.length() + 1];
		strcpy(name, uniformName.c_str());
		
		//Mise en place du champ
		ImGui::InputFloat(name, &value);
		//Si la valeur a ete modifiee on met a jour la valeur dans l'uniform
		if (it->second != value) {
			it->second = value;
			setUniformFloat(uniform.second, name, value);
		}

		
	}

	for (auto it = listUniformVec3.begin(); it != listUniformVec3.end(); ++it) {

		pair<string, GLenum> uniform = it->first;
		string uniformName = uniform.first;

		//On cree un tableau de float de taille 3 pour representer le vec3
		float *value = new float[3];
		glm::vec3 v = it->second;
		value[0] = v.x;
		value[1] = v.y;
		value[2] = v.z;

		char* name = new char[uniformName.length() + 1];
		strcpy(name, uniformName.c_str());

		ImGui::InputFloat3(name, value);

		if (it->second.x != value[0] || it->second.y != value[1] || it->second.z != value[2]) {
			it->second.x = value[0];
			it->second.y = value[1];
			it->second.z = value[2];
			setUniformVec3(uniform.second, name, it->second);
		}

	}

	for (auto it = listUniformVec4.begin(); it != listUniformVec4.end(); ++it) {

		pair<string, GLenum> uniform = it->first;
		string uniformName = uniform.first;

		//On cree un tableau de float de taille 4 pour representer le vec4
		float *value = new float[4];
		glm::vec4 v = it->second;
		value[0] = v.x;
		value[1] = v.y;
		value[2] = v.z;
		value[3] = v.w;

		char* name = new char[uniformName.length() + 1];
		strcpy(name, uniformName.c_str());

		ImGui::InputFloat4(name, value);

		if (it->second.x != value[0] || it->second.y != value[1] || it->second.z != value[2] || it->second.z != value[3]) {
			it->second.x = value[0];
			it->second.y = value[1];
			it->second.z = value[2];
			it->second.w = value[3];
			setUniformVec4(uniform.second, name, it->second);
		}

	}

	for (auto it = listUniformBool.begin(); it != listUniformBool.end(); ++it) {

		pair<string, GLenum> uniform = it->first;
		string uniformName = uniform.first;

		//On cree la variable booleen
		bool value = it->second;

		char* name = new char[uniformName.length() + 1];
		strcpy(name, uniformName.c_str());

		ImGui::Checkbox(name, &value);
		if (it->second != value) {
			it->second = value;
			setUniformBool(uniform.second, name, it->second);
		}

	}

}

//Ajout d'un uniforme de type float dans la liste
void BRDFMaterial::addUniformFloatInList(string uniformName, GLenum typeShader, float value) {

	if (!isUniformExist("float", uniformName, typeShader)) {

		pair<string, GLenum> p = pair<string, GLenum>(uniformName, typeShader);
		listUniformFloat.insert(pair<pair<string, GLenum>, float>(p, value));
	}
}

//Ajout d'un uniforme de type vec3 dans la liste
void BRDFMaterial::addUniformVec3InList(string uniformName, GLenum typeShader, glm::vec3 &value) {

	if (!isUniformExist("vec3", uniformName, typeShader)) {

		pair<string, GLenum> p = pair<string, GLenum>(uniformName, typeShader);
		listUniformVec3.insert(pair<pair<string, GLenum>, glm::vec3>(p, value));
	}
}

//Ajout d'un uniforme de type vec4 dans la liste
void BRDFMaterial::addUniformVec4InList(string uniformName, GLenum typeShader, glm::vec4 &value) {

	if (!isUniformExist("vec4", uniformName, typeShader)) {

		pair<string, GLenum> p = pair<string, GLenum>(uniformName, typeShader);
		listUniformVec4.insert(pair<pair<string, GLenum>, glm::vec4>(p, value));
	}
}

void BRDFMaterial::addUniformBoolInList(string uniformName, GLenum typeShader, bool value) {

	if (!isUniformExist("bool", uniformName, typeShader)) {

		pair<string, GLenum> p = pair<string, GLenum>(uniformName, typeShader);
		listUniformBool.insert(pair<pair<string, GLenum>, bool>(p, value));

	}
}

/*
	Méthode permettant de savoir si l'uniform passé en paramètre est déjà présent dans la liste
*/
bool BRDFMaterial::isUniformExist(string type, string uniformName, GLenum typeShader) {

	if (type == "float") {

		for (auto it = listUniformFloat.begin(); it != listUniformFloat.end(); ++it) {
			if (it->first.first == uniformName && it->first.second == typeShader)
				return true;
		}
		return false;

	}
	else if (type == "vec3") {

		for (auto it = listUniformVec3.begin(); it != listUniformVec3.end(); ++it) {
			if (it->first.first == uniformName && it->first.second == typeShader)
				return true;
		}
		return false;

	}
	else if (type == "vec4") {

		for (auto it = listUniformVec4.begin(); it != listUniformVec4.end(); ++it) {
			if (it->first.first == uniformName && it->first.second == typeShader)
				return true;
		}
		return false;

	}
	else if (type == "bool") {

		for (auto it = listUniformBool.begin(); it != listUniformBool.end(); ++it){
			if (it->first.first == uniformName && it->first.second == typeShader)
				return true;
		}
		return false;
	}
	
	//si le type est inconnu on renvoie true pour éviter de l'ajouter dans la liste
	return true;

}