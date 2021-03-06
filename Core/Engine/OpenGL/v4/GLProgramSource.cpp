/*
 *	(c) XLim, UMR-CNRS
 *	Authors: G.Gilet
 *  Documentation : J. Lemerle P. Le Gac
 */

#include <string.h>

#include "Engine/OpenGL/v4/GLProgramSource.h"
#include "Engine/OpenGL/Managers/textfile.h"

// for static init
map<string, const char*> GLProgramSource::m_IncludeSource;
bool GLProgramSource::noIncludes = true;

GLProgramSource::GLProgramSource(std::string name)
{

	m_Program = 0; //intialise l'adresse m?moire du shader ? 0 (signifie non existant)
	info_text = "";
	loaded = false; //shader non charg? pour l'instant
}

GLProgramSource::~GLProgramSource()
{

	glDeleteProgram(m_Program); //Fonction de suppression du shader dans opengl
}

void GLProgramSource::createNamedString(string name, string filename)
{
	string pfn(filename.c_str());
	char *shadertxt = textFileRead((char*)filename.c_str());

	const char *shadertxtc = shadertxt;
	// besoin d'un stockage des sources dans un map <name,shaderSource> pour un eventuel remplacement dans les shaders
	/*
	if (GLEW_ARB_shading_language_include)
	{
		glNamedStringARB(GL_SHADER_INCLUDE_ARB, -1, name.c_str(), -1, shadertxtc);
	}
	else
		*/
	{
		// Need to parse the named string to
		m_IncludeSource.insert(make_pair(name, shadertxtc));
		noIncludes = false;
	}
}

void GLProgramSource::deleteNamedString(string name)
{
	/*
	if (GLEW_ARB_shading_language_include)
	{
		glDeleteNamedStringARB(-1, name.c_str());
	}
	else
	*/
	{
		
		if (!noIncludes)
		{
			// Pour le moment on d?sactive : erreur de m?moire ? la fermeture de la fen?tre
			//if (GLProgramSource::m_IncludeSource.find(name) != GLProgramSource::m_IncludeSource.end())
			//{
			//	GLProgramSource::m_IncludeSource.erase(GLProgramSource::m_IncludeSource.find(name));
			//}
			//if (GLProgramSource::m_IncludeSource.size() == 0) noIncludes = true;
		}

	}
}

bool GLProgramSource::createProgram(GLenum shaderType, std::string filename)
{
	setFilename(filename); //mise ? jour de l'attribut qui stocke le chemin d'acc?s au fichier
	this->shaderType = shaderType; //type de shader ? passer ? opengl

	//Import du shader depuis le fichier
	char *shadertxt;
	string pfn(filename.c_str());
	shadertxt = textFileRead((char*)filename.c_str()); //r?cup?ration du contenu en tableau de char (null si probl?me ? l'import)
	shaderSource = string(shadertxt);

	//L?ve une exception si il y a eu un probl?me ? l'import du fichier (erreur chemin / taille du fichier)
	if (shadertxt == NULL)
		throw std::logic_error(string("ERROR : GLProgram : Error Reading Source File\n") + filename + string("\n"));


	//Cr?ation d'un emplacement m?moire vide pour le shader dans opengl et r?cup?ration de l'adresse associ?e
	const GLuint shader = glCreateShader(shaderType);

	/*
	if (GLEW_ARB_shading_language_include)
	{
		const char *shadertxtc = shadertxt;
		glShaderSource(shader, 1, &shadertxtc, NULL);
		string rootPath = "/";
		const char *SourceString = rootPath.c_str();
		glCompileShaderIncludeARB(shader, 1, &SourceString, NULL);

	}
	
	else
	*/
	{
		//string extensionTest = string("#extension GL_ARB_shading_language_include : enable");
		//string include = string("#include");
		//
		//std::size_t found = 0;
		//
		//std::size_t lastPosition = 0;
		//bool parsing = true;
		//while (parsing)
		//{
		//	string* currentlyParsed = new string();
		//	string toSearch = string("#include \""); // searching the include
		//	found = shaderSource.find(toSearch, found);
		//	if (found != std::string::npos)
		//	{
		//		// storing the current string in array
		//		int currentLength = found - lastPosition - 1;
		//		if (currentLength != 0)
		//		{
		//			*currentlyParsed = (shaderSource.substr(lastPosition, currentLength));
		//			currentlyParsed->append("\0");
		//			listOfSources.push_back(currentlyParsed->c_str());
		//			listOfLength.push_back(currentlyParsed->length());
		//		}
		//		// Search the end of the current include
		//		std::size_t nextLineAt = shaderSource.find("\"", found + 11);
		//		string currentInclude = shaderSource.substr(found + 10, nextLineAt - (found + 10));
		//		std::map<string, const char*>::iterator currentIncludeIt = m_IncludeSource.find(currentInclude);
		//		if (currentIncludeIt != m_IncludeSource.end())
		//		{
		//			// storing the include's source in the source array
		//			listOfSources.push_back(currentIncludeIt->second);
		//			listOfLength.push_back(strlen(currentIncludeIt->second));
		//		}
		//		// nextline to parse
		//		lastPosition = shaderSource.find("\n", found);
		//		found = lastPosition;
		//
		//	}
		//	else
		//	{
		//		// nothing else to parse : storing the rest of the source file
		//		*currentlyParsed = shaderSource.substr(lastPosition);
		//		currentlyParsed->append("\0");
		//		listOfSources.push_back(currentlyParsed->c_str());
		//		listOfLength.push_back(currentlyParsed->length());
		//		parsing = false;
		//	}
		//}

		//r?cup?re tous les includes du shader source et les ajoutent ainsi que le shader lui m?me dans listOfSources 
		this->parseSources(shaderSource, &listOfSources, &listOfLength);

		//chargement du shader et des sources d'includes associ?s ? l'emplacement @shader cr?? pr?c?demment dans opengl
		glShaderSource(shader, listOfLength.size(), &listOfSources[0], &listOfLength[0]);

		//compilation du shader qui vient d'?tre charg?
		glCompileShader(shader);
	}

	m_Program = glCreateProgram(); // cr?ation d'un programme opengl vide auquel on va lier le shader compil?

	//si l'emplacement a ?t? cr??...
	if (m_Program) {
		GLint compiled = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled); //r?cup?re statut indiquant si le shader a bien ?t? compil?
		glProgramParameteri(m_Program, GL_PROGRAM_SEPARABLE, GL_TRUE);
		if (compiled) {
			glAttachShader(m_Program, shader);
			glLinkProgram(m_Program); //cr?? un ex?cutable du shader qui sera utilis? par le processeur ad?quat associ?
			glDetachShader(m_Program, shader);
		}
		else
		{
			printShaderInfoLog(shader);
		}
	}
	glDeleteShader(shader); // lib?ration dans opengl de la m?moire o? est stock? le shader qui a ?t? compil?

	free(shadertxt); // lib?ration de la m?moire o? est stock? le contenu du shader

	loaded = isValid(); // regarde si le program a bien ?t? charg? dans opengl

	//si le program n'a pa ?t? correctement charg?, affichage des erreurs dans la console et lev?e d'une erreur / exception
	if (!loaded)
	{
		printProgramInfoLog();
		string err = printErrorString();
		throw std::logic_error(err);
	}

	//retourne le statut du programme charg? dans opengl
	return loaded;

}

void GLProgramSource::printProgramInfoLog()
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH,&infologLength);
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(m_Program, infologLength, &charsWritten, infoLog);
		string df(infoLog);
		info_text += df ;
		info_text += "\n";
		free(infoLog);
	}
}

void GLProgramSource::printShaderInfoLog(GLuint shader)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		string df(infoLog);
		info_text += df ;
		info_text += "\n";
		free(infoLog);
	}
}

string GLProgramSource::printErrorString()
{
	string err;
	err += "ERROR : GLProgramSource : Error Creating ";
	if (shaderType==GL_VERTEX_SHADER)
		err += "Vertex Shader ";
	else if (shaderType==GL_TESS_CONTROL_SHADER)
		err += "Tesselation Control Shader ";
	else if (shaderType==GL_TESS_EVALUATION_SHADER)
		err += "Tesselation Evaluation Shader ";
	else if (shaderType==GL_GEOMETRY_SHADER)
		err += "Geometry Shader ";
	else if (shaderType==GL_FRAGMENT_SHADER)
		err += "Fragment Shader ";
	else if (shaderType==GL_COMPUTE_SHADER)
		err += "Compute Shader ";
	else
		err += "Shader ";

	err += m_Filename ;
	err += " : \n";
	err += info_text;
	return err;
}


bool GLProgramSource::isValid()
{
	GLint status;
	glGetProgramiv(m_Program,GL_VALIDATE_STATUS,&status);
	return(status==GL_TRUE);
}

GLuint GLProgramSource::getProgram()
{
	return(m_Program);
}

void GLProgramSource::setFilename(string pathfilename)
{
	m_Filename = pathfilename;
}


void GLProgramSource::parseSources(string analysedSource, vector<const char*>* ptrlistOfSources, vector<int>* ptrlistOfLength)
{
	string extensionTest = string("#extension GL_ARB_shading_language_include : enable");
	string include = string("#include");

	std::size_t found = 0;

	std::size_t lastPosition = 0;
	bool parsing = true;
	while (parsing)
	{
		string* currentlyParsed = new string();
		string toSearch = string("#include \""); // searching the include
		found = analysedSource.find(toSearch, found);
		if (found != std::string::npos) // Si la position retourn?e n'est pas ? la fin du string
		{
			// storing the current string in array
			int currentLength = found - lastPosition - 1;
			if (currentLength != 0)
			{
				*currentlyParsed = (analysedSource.substr(lastPosition, currentLength));
				currentlyParsed->append("\0");
				ptrlistOfSources->push_back(currentlyParsed->c_str()); //ajoute le code source de l'include dans la liste des sources
				ptrlistOfLength->push_back(currentlyParsed->length()); //m?morise la taille du fichier include source dans le tableau (n?c?ssaire pour cr?er le shader dans opengl)
			}
			// Search the end of the current include
			std::size_t nextLineAt = analysedSource.find("\"", found + 11);
			string currentInclude = analysedSource.substr(found + 10, nextLineAt - (found + 10));

			//Regarde si l'include n'est pas d?j? pr?sent dans la liste d'include
			std::map<string, const char*>::iterator currentIncludeIt = m_IncludeSource.find(currentInclude);
			if (currentIncludeIt != m_IncludeSource.end()) //si il est pr?sent, on appelle le traitement r?cursif d'include sur ce dernier
			{
				// Parsing the include source for recursive includes
				this->parseSources(string(currentIncludeIt->second), ptrlistOfSources, ptrlistOfLength);
				// storing the include's source in the source array
				//ptrlistOfSources->push_back(currentIncludeIt->second);
				//ptrlistOfLength->push_back(strlen(currentIncludeIt->second));
			}
			// nextline to parse
			lastPosition = analysedSource.find("\n", found);
			found = lastPosition;

		}
		else
		{
			// nothing else to parse : storing the rest of the source file
			*currentlyParsed = analysedSource.substr(lastPosition);
			currentlyParsed->append("\0");
			ptrlistOfSources->push_back(currentlyParsed->c_str());
			ptrlistOfLength->push_back(currentlyParsed->length());
			parsing = false;
		}
	}
}