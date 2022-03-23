/*
	(c) XLim, UMR-CNRS
	Authors: J.Lemerle
	Documentation : J. Lemerle P. Le Gac
*/

#include "MaterialLoader.hpp"
#include "TemplateShader.hpp"

#include <iostream>
#include <fstream>

using namespace std;
using json = nlohmann::json;

BRDFMaterial* MaterialLoader::loadMaterialFromJSON(string filename) {
	
	//Chargement du fichier via flux dans un objet json
	ifstream input_stream(filename);
	json json_file;
	input_stream >> json_file;
	
	map<string, float> float_parameters;
	map<string, glm::vec4> vec4_parameters;
	map<string, glm::vec3> vec3_parameters;
	map<string, bool> bool_parameters;

	map<string, string> vs_snippets;
	map<string, string> fs_snippets;
	vs_snippets["includes"] = "";
	vs_snippets["uniforms"] = "";
	vs_snippets["in_out"] = "";
	vs_snippets["layout"] = "";
	vs_snippets["main_code"] = "";

	fs_snippets["includes"] = "";
	fs_snippets["uniforms"] = "";
	fs_snippets["in_out"] = "";
	fs_snippets["layout"] = "";
	fs_snippets["main_code"] = "";


	//Récupération de la normale automatique (pour l'instant), à changer si ajout gestion normales via textures, deformation, etc...
	vs_snippets["layout"] += "layout (location = 2) in vec3 Normale;\n";
	vs_snippets["main_code"] += "N = normalize((NormalMV * vec4(Normale, 0.0)).xyz);\n";


	//Premier paramètre : couleur du matérial
	glm::vec4 color;

	//si paramètre trouvé
	if (json_file.find("color") != json_file.end()) {
		color = glm::vec4(json_file["color"][0].get<float>(),
			json_file["color"][1].get<float>(),
			json_file["color"][2].get<float>(),
			json_file["color"][3].get<float>()
		);
	}
	//Sinon valeur par défaut
	else {
		color = glm::vec4(1.f, 0.f, 0.f, 1.f);
	}
	vec4_parameters["color"] = color;
	fs_snippets["uniforms"] += "vec4 color;\n";

	//Second paramètre : coefficient composante diffuse
	float kd;

	//si paramètre trouvé
	if (json_file.find("kd") != json_file.end()) {
		kd = json_file["kd"].get<float>();
	}
	//Sinon valeur par défaut
	else {
		kd = 0.6f;
	}
	float_parameters["kd"] = kd;
	fs_snippets["uniforms"] += "float kd;\n";


	//Troisième paramètre : coefficient composante diffuse
	float ks;

	//si paramètre trouvé
	if (json_file.find("ks") != json_file.end()) {
		ks = json_file["ks"].get<float>();
	}
	//Sinon valeur par défaut
	else {
		ks = 0.4f;
	}
	float_parameters["ks"] = ks;
	fs_snippets["uniforms"] += "float ks;\n";

	//Quatrième paramètre : fonction de brdf (éclairage)
	string brdf_name;

	//si brdf trouvée
	if (json_file.find("brdf") != json_file.end()) {
		json brdf = json_file["brdf"];

		//Cas des BRDF "spéciales"
		if (brdf.find("diffuse_specular") != brdf.end()) {

			json other = brdf["diffuse_specular"];
			json params;
			json default_config;

			


			//BRDF d'Ashikhman-Shirley
			if (other["name"].get<string>() == "ashikhman_shirley") {
				fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/other/AshikhmanShirley\"\n";

				vs_snippets["layout"] += "layout(location = 4) in vec3 Tangent;\n";
				vs_snippets["in_out"] += "out vec3 X;\n";
				vs_snippets["main_code"] += "X = Tangent;\n";

				fs_snippets["in_out"] += "in vec3 X;\n";
				fs_snippets["main_code"] += "vec3 Y = cross(N, X);\n";
				fs_snippets["main_code"] += "Color = vec4(0.0, 0.0, 0.0, 1.0);\n";
				fs_snippets["main_code"] += " for(int i = 0 ; i < nbLights.x; ++i){\n";
				fs_snippets["main_code"] += "	vec3 L = normalize(Lights[i].pos.xyz - pointPosition);\n";

				//Paramètres d'Ashikhman-Shirley
				float Rs;
				float Rd;
				float nu;
				float nv;
				bool isotropic;
				bool coupled_diffuse;
				ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/other/AshikhmanShirley_default.json");
				input_stream >> default_config;

				if (other.find("parameters") != other.end()) {
					params = other["parameters"];
					if (params.find("Rs") != params.end())
						Rs = params["Rs"].get<float>();
					else
						Rs = default_config["Rs"].get<float>();

					if (params.find("Rd") != params.end())
						Rd = params["Rd"].get<float>();
					else
						Rd = default_config["Rd"].get<float>();

					if (params.find("nu") != params.end())
						nu = params["nu"].get<float>();
					else
						nu = default_config["nu"].get<float>();

					if (params.find("nv") != params.end())
						nv = params["nv"].get<float>();
					else
						nv = default_config["nv"].get<float>();

					if (params.find("nv") != params.end())
						nv = params["nv"].get<float>();
					else
						nv = default_config["nv"].get<float>();

					if (params.find("isotropic") != params.end())
						isotropic = params["isotropic"].get<bool>();
					else
						isotropic = default_config["isotropic"].get<bool>();

					if (params.find("coupled_diffuse") != params.end())
						coupled_diffuse = params["coupled_diffuse"].get<bool>();
					else
						coupled_diffuse = default_config["coupled_diffuse"].get<bool>();
				}
				else {
					Rs = default_config["Rs"].get<float>();
					Rd = default_config["Rd"].get<float>();
					nu = default_config["nu"].get<float>();
					nv = default_config["nv"].get<float>();
					isotropic = default_config["isotropic"].get<bool>();
					coupled_diffuse = default_config["coupled_diffuse"].get<bool>();

				}

				//Ajout des paramètres dans les uniforms du fragment et dans le tableau pour l'instance de Material
				float_parameters["Rd"] = Rd;
				float_parameters["Rs"] = Rs;
				float_parameters["nu"] = nu;
				float_parameters["nv"] = nv;
				bool_parameters["isotropic"] = isotropic;
				bool_parameters["coupled_diffuse"] = coupled_diffuse;

				fs_snippets["uniforms"] += "float Rs;\n";
				fs_snippets["uniforms"] += "float Rd;\n";
				fs_snippets["uniforms"] += "float nu;\n";
				fs_snippets["uniforms"] += "float nv;\n";
				fs_snippets["uniforms"] += "bool isotropic;\n";
				fs_snippets["uniforms"] += "bool coupled_diffuse;\n";


				fs_snippets["main_code"] += "float value = BRDFAshikhmanShirley(L,V,N, X, Y, Rs, Rd, nu, nv, isotropic, coupled_diffuse);\n";
				fs_snippets["main_code"] += "Color += color * value;\n";
				fs_snippets["main_code"] += "}\n";
			}

			//BRDF de Walter
			else if (other["name"].get<string>() == "walter") {
				fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/other/Walter\"\n";


				fs_snippets["main_code"] += "Color = vec4(0.0, 0.0, 0.0, 1.0);\n";
				fs_snippets["main_code"] += " for(int i = 0 ; i < nbLights.x; ++i){\n";
				fs_snippets["main_code"] += "	vec3 L = normalize(Lights[i].pos.xyz - pointPosition);\n";

				//Paramètres de Walter
				float alphaG;
				float ior;
				bool useFresnel;
				ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/other/Walter_default.json");
				input_stream >> default_config;

				if (other.find("parameters") != other.end()) {
					params = other["parameters"];
					if (params.find("alphaG") != params.end())
						alphaG = params["alphaG"].get<float>();
					else
						alphaG = default_config["alphaG"].get<float>();

					if (params.find("ior") != params.end())
						ior = params["ior"].get<float>();
					else
						ior = default_config["ior"].get<float>();

					
					if (params.find("useFresnel") != params.end())
						useFresnel = params["useFresnel"].get<bool>();
					else
						useFresnel = default_config["useFresnel"].get<bool>();

				}
				else {
					alphaG = default_config["alphaG"].get<float>();
					ior = default_config["ior"].get<float>();

					useFresnel = default_config["useFresnel"].get<bool>();

				}

				//Ajout des paramètres dans les uniforms du fragment et dans le tableau pour l'instance de Material
				float_parameters["alphaG"] = alphaG;
				float_parameters["ior"] = ior;
				bool_parameters["useFresnel"] = useFresnel;

				fs_snippets["uniforms"] += "float alphaG;\n";
				fs_snippets["uniforms"] += "float ior;\n";
				fs_snippets["uniforms"] += "bool useFresnel;\n";



				fs_snippets["main_code"] += "float value =  BRDFWalter(L, V, N, kd, ks, alphaG, ior, useFresnel);\n";
				fs_snippets["main_code"] += "Color += color * value;\n";
				fs_snippets["main_code"] += "}\n";
			}

			//BRDF de Ward
			else if (other["name"].get<string>() == "ward") {
				fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/other/Ward\"\n";

				vs_snippets["layout"] += "layout(location = 4) in vec3 Tangent;\n";
				vs_snippets["in_out"] += "out vec3 X;\n";
				vs_snippets["main_code"] += "X = Tangent;\n";

				fs_snippets["in_out"] += "in vec3 X;\n";
				fs_snippets["main_code"] += "vec3 Y = cross(N, X);\n";
				fs_snippets["main_code"] += "Color = vec4(0.0, 0.0, 0.0, 1.0);\n";
				fs_snippets["main_code"] += " for(int i = 0 ; i < nbLights.x; ++i){\n";
				fs_snippets["main_code"] += "	vec3 L = normalize(Lights[i].pos.xyz - pointPosition);\n";

				//Paramètres de Ward
				float alphaX;
				float alphaY;
				bool isotropic;
				glm::vec4 cs;
				glm::vec4 cd;
				ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/other/Ward_default.json");
				input_stream >> default_config;

				if (other.find("parameters") != other.end()) {
					params = other["parameters"];
					if (params.find("alphaX") != params.end())
						alphaX = params["alphaX"].get<float>();
					else
						alphaX = default_config["alphaX"].get<float>();

					if (params.find("alphaY") != params.end())
						alphaY = params["alphaY"].get<float>();
					else
						alphaY = default_config["alphaY"].get<float>();


					if (params.find("isotropic") != params.end())
						isotropic = params["isotropic"].get<bool>();
					else
						isotropic = default_config["isotropic"].get<bool>();

					if (params.find("Cs") != params.end()) {
						cs = glm::vec4(params["Cs"][0].get<float>(),
							params["Cs"][1].get<float>(),
							params["Cs"][2].get<float>(),
							params["Cs"][3].get<float>()
						);
					}
					else {
						cs = glm::vec4(default_config["Cs"][0].get<float>(),
							default_config["Cs"][1].get<float>(),
							default_config["Cs"][2].get<float>(),
							default_config["Cs"][3].get<float>()
						);
					}

					if (params.find("Cd") != params.end()) {
						cd = glm::vec4(params["Cd"][0].get<float>(),
							params["Cd"][1].get<float>(),
							params["Cd"][2].get<float>(),
							params["Cd"][3].get<float>()
						);
					}
					else {
						cd = glm::vec4(default_config["Cd"][0].get<float>(),
							default_config["Cd"][1].get<float>(),
							default_config["Cd"][2].get<float>(),
							default_config["Cd"][3].get<float>()
						);
					}

				}
				else {
					alphaX = default_config["alphaX"].get<float>();
					alphaY = default_config["alphaY"].get<float>();

					isotropic = default_config["isotropic"].get<bool>();

					cs = glm::vec4(default_config["Cs"][0].get<float>(),
						default_config["Cs"][1].get<float>(),
						default_config["Cs"][2].get<float>(),
						default_config["Cs"][3].get<float>()
					);

					cd = glm::vec4(default_config["Cd"][0].get<float>(),
						default_config["Cd"][1].get<float>(),
						default_config["Cd"][2].get<float>(),
						default_config["Cd"][3].get<float>()
					);

				}

				//Ajout des paramètres dans les uniforms du fragment et dans le tableau pour l'instance de Material
				float_parameters["alphaX"] = alphaX;
				float_parameters["alphaY"] = alphaY;
				bool_parameters["isotropic"] = isotropic;
				vec4_parameters["cs"] = cs;
				vec4_parameters["cd"] = cd;

				fs_snippets["uniforms"] += "float alphaX;\n";
				fs_snippets["uniforms"] += "float alphaY;\n";
				fs_snippets["uniforms"] += "bool isotropic;\n";
				fs_snippets["uniforms"] += "vec4 cs;\n";
				fs_snippets["uniforms"] += "vec4 cd;\n";

				fs_snippets["main_code"] += "vec3 value =  BRDFWard(L, V, N, X, Y, alphaX, alphaY, cs, cd, isotropic);\n";
				fs_snippets["main_code"] += "Color += vec4(value,1);\n";
				fs_snippets["main_code"] += "}\n";
			}

		}

		//Traitement de la partie diffuse du materiau
		else if (brdf.find("diffuse") == brdf.end()) {
			brdf_name = "flat_color";
			fs_snippets["main_code"] += "Color = color;\n";
		}
		else {

			json default_config;

			fs_snippets["main_code"] += "Color = vec4(0.0, 0.0, 0.0, 1.0);\n";
			fs_snippets["main_code"] += " for(int i = 0 ; i < nbLights.x; ++i){\n";
			fs_snippets["main_code"] += "	vec3 L = normalize(Lights[i].pos.xyz - pointPosition);\n";


			json diffuse = brdf["diffuse"];
			if (diffuse["name"].get<string>() == "lambert") {
				fs_snippets["main_code"] += "	float diffuse_value = 1;\n";

			}
			else if (diffuse["name"].get<string>() == "orenNayar") {
				fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/diffuse/OrenNayar\"\n";

				//Paramètres d'Oren Nayar : rho et sigma
				float rho;
				float sigma;

				ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/diffuse/OrenNayar_default.json");
				input_stream >> default_config;

				if (diffuse.find("parameters") != diffuse.end()) {
					if (diffuse["parameters"].find("rho") != diffuse["parameters"].end())
						rho = diffuse["parameters"]["rho"].get<float>();
					else
						rho = default_config["rho"].get<float>();

					if (diffuse["parameters"].find("sigma") != diffuse["parameters"].end())
						sigma = diffuse["parameters"]["sigma"].get<float>();
					else
						sigma = default_config["sigma"].get<float>();
				}
				else {
					rho = default_config["rho"].get<float>();
					sigma = default_config["sigma"].get<float>();
				}

				//Ajout des paramètres dans les uniforms du fragment et dans le tableau pour l'instance de Material
				float_parameters["rho"] = rho;
				float_parameters["sigma"] = sigma;

				fs_snippets["uniforms"] += "float rho;\n";
				fs_snippets["uniforms"] += "float sigma;\n";

				fs_snippets["main_code"] += "float diffuse_value = BRDFOrenNayar(N,V,L,rho,sigma);\n";
			}

			//Diffuse de Minnaert
			else if (diffuse["name"] == "minnaert") {
				json spec_param;

				fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/diffuse/Minnaert\"\n";

				ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/diffuse/Minnaert_default.json");
				input_stream >> default_config;

				//Récupération des paramètres de Minnaert
				float k;
				if (diffuse.find("parameters") != diffuse.end()) {
					spec_param = diffuse["parameters"];
					if (spec_param.find("k") != spec_param.end())
						k = spec_param["k"].get<float>();
					else
						k = default_config["k"].get<float>();

				}
				else {
					k = default_config["k"].get<float>();
				}

				//ajout des paramètres uniforms
				float_parameters["k"] = k;
				fs_snippets["uniforms"] += "float k;\n";

				fs_snippets["main_code"] += "	float diffuse_value = BRDFMinnaert(L, V, N, k);\n";
			}

			fs_snippets["main_code"] += "vec4 diffuse_composante = color * max(0, dot(N,L)) * diffuse_value * kd;\n";

			//Traitement de la composante spéculaire
			if (brdf.find("specular") == brdf.end()) {
				fs_snippets["main_code"] += "float specular_value = 0.;\n";
			}
			else {
				json specular = brdf["specular"];
				json spec_param;

				//Spéculaire de Phong
				if (specular["name"] == "phong") {

					fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/reflectance/phong\"\n";

					ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/reflectance/Phong_default.json");
					input_stream >> default_config;

					//Récupération des paramètres de phong
					float spec_pow;
					if (specular.find("parameters") != specular.end()) {
						spec_param = specular["parameters"];
						if (spec_param.find("spec_pow") != spec_param.end())
							spec_pow = spec_param["spec_pow"].get<float>();
						else
							spec_pow = default_config["spec_pow"].get<float>();
					}
					else
						spec_pow = default_config["spec_pow"].get<float>();

					//ajout des paramètres uniforms
					float_parameters["spec_pow"] = spec_pow;
					fs_snippets["uniforms"] += "float spec_pow;\n";
					
					fs_snippets["main_code"] += "	float specular_value = phongBRDF(N,V,L,spec_pow);\n";
				}

				//Spéculaire de Beckmann
				else if (specular["name"] == "beckmann") {

					fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/reflectance/Beckmann\"\n";

					ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/reflectance/Beckmann_default.json");
					input_stream >> default_config;

					//Récupération des paramètres de beckmann
					float m;
					if (specular.find("parameters") != specular.end()) {
						spec_param = specular["parameters"];
						if (spec_param.find("m") != spec_param.end())
							m = spec_param["m"].get<float>();
						else
							m = default_config["m"].get<float>();
					}
					else
						m = default_config["m"].get<float>();

					//ajout des paramètres uniforms
					float_parameters["m"] = m;
					fs_snippets["uniforms"] += "float m;\n";

					fs_snippets["main_code"] += "	float specular_value = BRDFBeckmann(L, V, N,,m);\n";
				}

				//Spéculaire de Blinn
				else if (specular["name"] == "blinn") {

					fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/reflectance/Blinn\"\n";

					ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/reflectance/Blinn_default.json");
					input_stream >> default_config;

					//Récupération des paramètres de Blinn
					float n;
					float ior;
					bool include_Fresnel;
					bool divide_by_NdotL;
					if (specular.find("parameters") != specular.end()) {
						spec_param = specular["parameters"];
						if (spec_param.find("n") != spec_param.end())
							n = spec_param["n"].get<float>();
						else
							n = default_config["n"].get<float>();

						if (spec_param.find("ior") != spec_param.end())
							ior = spec_param["ior"].get<float>();
						else
							ior = default_config["ior"].get<float>();

						if (spec_param.find("include_Fresnel") != spec_param.end())
							include_Fresnel = spec_param["include_Fresnel"].get<bool>();
						else
							include_Fresnel = default_config["include_Fresnel"].get<bool>();

						if (spec_param.find("divide_by_NdotL") != spec_param.end())
							divide_by_NdotL = spec_param["divide_by_NdotL"].get<bool>();
						else
							divide_by_NdotL = default_config["divide_by_NdotL"].get<bool>();
					}
					else {
						n = default_config["n"].get<float>();
						ior = default_config["ior"].get<float>();
						include_Fresnel = default_config["include_Fresnel"].get<bool>();
						divide_by_NdotL = default_config["divide_by_NdotL"].get<bool>();
					}
						

					//ajout des paramètres uniforms
					float_parameters["n"] = n;
					fs_snippets["uniforms"] += "float n;\n";
					float_parameters["ior"] = ior;
					fs_snippets["uniforms"] += "float ior;\n";
					bool_parameters["include_Fresnel"] = include_Fresnel;
					fs_snippets["uniforms"] += "bool include_Fresnel;\n";
					bool_parameters["divide_by_NdotL"] = divide_by_NdotL;
					fs_snippets["uniforms"] += "bool divide_by_NdotL;\n";

					fs_snippets["main_code"] += "	float specular_value = BRDFBlinn(L, V, N, n, ior, include_Fresnel, divide_by_NdotL);\n";
				}

				//Spéculaire de Blinn-Phong
				else if (specular["name"] == "blinn_phong") {

					fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/reflectance/BlinnPhong\"\n";

					ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/reflectance/BlinnPhong_default.json");
					input_stream >> default_config;

					//Récupération des paramètres de Blinn Phong
					float n;
					bool divide_by_NdotL;
					if (specular.find("parameters") != specular.end()) {
						spec_param = specular["parameters"];
						if (spec_param.find("n") != spec_param.end())
							n = spec_param["n"].get<float>();
						else
							n = default_config["n"].get<float>();
						
						if (spec_param.find("divide_by_NdotL") != spec_param.end())
							divide_by_NdotL = spec_param["divide_by_NdotL"].get<bool>();
						else
							divide_by_NdotL = default_config["divide_by_NdotL"].get<bool>();
					}
					else {
						n = default_config["n"].get<float>();
						divide_by_NdotL = default_config["divide_by_NdotL"].get<bool>();
					}


					//ajout des paramètres uniforms
					float_parameters["n"] = n;
					fs_snippets["uniforms"] += "float n;\n";

					bool_parameters["divide_by_NdotL"] = divide_by_NdotL;
					fs_snippets["uniforms"] += "bool divide_by_NdotL;\n";

					fs_snippets["main_code"] += "	float specular_value = BRDFBlinnPhong(L, V, N, n, divide_by_NdotL);\n";
				}

				//Spéculaire de Cook Torrance
				else if (specular["name"] == "cook_torrance") {

					fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/reflectance/CookTorrance\"\n";

					ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/reflectance/CookTorrance_default.json");
					input_stream >> default_config;

					//Récupération des paramètres de Cook Torrance
					float m;
					float f0;
					bool include_F;
					bool include_G;
					if (specular.find("parameters") != specular.end()) {
						spec_param = specular["parameters"];
						if (spec_param.find("m") != spec_param.end())
							m = spec_param["m"].get<float>();
						else
							m = default_config["m"].get<float>();

						if (spec_param.find("f0") != spec_param.end())
							f0 = spec_param["f0"].get<float>();
						else
							f0 = default_config["f0"].get<float>();

						if (spec_param.find("include_F") != spec_param.end())
							include_F = spec_param["include_F"].get<bool>();
						else
							include_F = default_config["include_F"].get<bool>();

						if (spec_param.find("include_G") != spec_param.end())
							include_G = spec_param["include_G"].get<bool>();
						else
							include_G = default_config["include_G"].get<bool>();
					}
					else {
						m = default_config["m"].get<float>();
						f0 = default_config["f0"].get<float>();
						include_F = default_config["include_F"].get<bool>();
						include_G = default_config["include_G"].get<bool>();
					}


					//ajout des paramètres uniforms
					float_parameters["m"] = m;
					fs_snippets["uniforms"] += "float m;\n";
					float_parameters["f0"] = f0;
					fs_snippets["uniforms"] += "float f0;\n";
					bool_parameters["include_F"] = include_F;
					fs_snippets["uniforms"] += "bool include_F;\n";
					bool_parameters["include_G"] = include_G;
					fs_snippets["uniforms"] += "bool include_G;\n";

					fs_snippets["main_code"] += "	float specular_value = BRDFCookTorrance(L, V, N, m, f0, include_F, include_G);\n";
				}

				//Spéculaire de Exponential
				else if (specular["name"] == "exponential") {

					fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/reflectance/Exponential\"\n";

					ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/reflectance/Exponential_default.json");
					input_stream >> default_config;

					//Récupération des paramètres de Exponential
					float c;
					bool normalized;
					if (specular.find("parameters") != specular.end()) {
						spec_param = specular["parameters"];
						if (spec_param.find("c") != spec_param.end())
							c = spec_param["c"].get<float>();
						else
							c = default_config["c"].get<float>();

						if (spec_param.find("normalized") != spec_param.end())
							normalized = spec_param["normalized"].get<bool>();
						else
							normalized = default_config["normalized"].get<bool>();
					}
					else {
						c = default_config["c"].get<float>();
						normalized = default_config["normalized"].get<bool>();
					}


					//ajout des paramètres uniforms
					float_parameters["c"] = c;
					fs_snippets["uniforms"] += "float c;\n";

					bool_parameters["normalized"] = normalized;
					fs_snippets["uniforms"] += "bool normalized;\n";

					fs_snippets["main_code"] += "	float specular_value = BRDFExponential(L, V, N, c, normalized);\n";
				}

				//Spéculaire de GGX
				else if (specular["name"] == "ggx") {

					fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/reflectance/GGX\"\n";

					ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/reflectance/GGX_default.json");
					input_stream >> default_config;

					//Récupération des paramètres de GGX
					float alpha;
					if (specular.find("parameters") != specular.end()) {
						spec_param = specular["parameters"];
						if (spec_param.find("alpha") != spec_param.end())
							alpha = spec_param["alpha"].get<float>();
						else
							alpha = default_config["alpha"].get<float>();

					
					}
					else {
						alpha = default_config["alpha"].get<float>();
					}


					//ajout des paramètres uniforms
					float_parameters["alpha"] = alpha;
					fs_snippets["uniforms"] += "float alpha;\n";


					fs_snippets["main_code"] += "	float specular_value = BRDFGGX(L, V, N, alpha);\n";
				}


				//Spéculaire de Nishino
				else if (specular["name"] == "nishino") {

					fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/reflectance/Nishino\"\n";

					ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/reflectance/Nishino_default.json");
					input_stream >> default_config;

					//Récupération des paramètres de Nishino
					float k;
					float n;
					if (specular.find("parameters") != specular.end()) {
						spec_param = specular["parameters"];
						if (spec_param.find("k") != spec_param.end())
							k = spec_param["k"].get<float>();
						else
							k = default_config["k"].get<float>();

						if (spec_param.find("n") != spec_param.end())
							n = spec_param["n"].get<float>();
						else
							n = default_config["n"].get<float>();



					}
					else {
						k = default_config["k"].get<float>();
						n = default_config["n"].get<float>();

					}


					//ajout des paramètres uniforms
					float_parameters["k"] = k;
					fs_snippets["uniforms"] += "float k;\n";
					float_parameters["n"] = n;
					fs_snippets["uniforms"] += "float n;\n";


					fs_snippets["main_code"] += "	float specular_value = BRDFNishino(L, V, N, k, n);\n";
				}

				//Spéculaire de StretchedPhong
				else if (specular["name"] == "stretched_phong") {

					fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/reflectance/StretchedPhong\"\n";

					ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/reflectance/StretchedPhong_default.json");
					input_stream >> default_config;

					//Récupération des paramètres de StretchedPhong
					float Ks;
					float n;
					float p;
					if (specular.find("parameters") != specular.end()) {
						spec_param = specular["parameters"];
						if (spec_param.find("Ks") != spec_param.end())
							Ks = spec_param["Ks"].get<float>();
						else
							Ks = default_config["Ks"].get<float>();

						if (spec_param.find("n") != spec_param.end())
							n = spec_param["n"].get<float>();
						else
							n = default_config["n"].get<float>();

						if (spec_param.find("p") != spec_param.end())
							p = spec_param["p"].get<float>();
						else
							p = default_config["p"].get<float>();



					}
					else {
						Ks = default_config["Ks"].get<float>();
						n = default_config["n"].get<float>();
						p = default_config["p"].get<float>();

					}


					//ajout des paramètres uniforms
					float_parameters["Ks"] = Ks;
					fs_snippets["uniforms"] += "float Ks;\n";
					float_parameters["n"] = n;
					fs_snippets["uniforms"] += "float n;\n";
					float_parameters["p"] = p;
					fs_snippets["uniforms"] += "float p;\n";


					fs_snippets["main_code"] += "	float specular_value = BRDFStretchedPhong(L, V, N, Ks, n, p);\n";
				}

				//Spéculaire de Trowbridge
				else if (specular["name"] == "trowbridge") {

					fs_snippets["includes"] += "#include \"/Materials/Common/BRDF/reflectance/Trowbridge\"\n";

					ifstream input_stream(ressourceCoreMaterialPath + "Common/BRDF/reflectance/Trowbridge_default.json");
					input_stream >> default_config;

					//Récupération des paramètres de Exponential
					float c;
					bool normalized;
					if (specular.find("parameters") != specular.end()) {
						spec_param = specular["parameters"];
						if (spec_param.find("c") != spec_param.end())
							c = spec_param["c"].get<float>();
						else
							c = default_config["c"].get<float>();

						if (spec_param.find("normalized") != spec_param.end())
							normalized = spec_param["normalized"].get<bool>();
						else
							normalized = default_config["normalized"].get<bool>();
					}
					else {
						c = default_config["c"].get<float>();
						normalized = default_config["normalized"].get<bool>();
					}


					//ajout des paramètres uniforms
					float_parameters["c"] = c;
					fs_snippets["uniforms"] += "float c;\n";

					bool_parameters["normalized"] = normalized;
					fs_snippets["uniforms"] += "bool normalized;\n";

					fs_snippets["main_code"] += "	float specular_value = BRDFTrowbridge(L, V, N, c, normalized);\n";
				}

			}
		

			fs_snippets["main_code"] += "	vec4 specular_composante = color * specular_value * ks;\n";
			fs_snippets["main_code"] += "Color += specular_composante + diffuse_composante;\n";
			fs_snippets["main_code"] += "}\n";
		}

	}
	//Sinon valeur par défaut
	else {
		brdf_name = "flat_color";
		fs_snippets["main_code"] += "Color = color;\n";
	}

	cout << "Couleur material : "<< color.r << " " << color.g << " " << color.b << " " << color.a << endl;
	cout << "Coefficient diffus kd : " << kd << endl;
	cout << "Coefficient spéculaire ks : " << ks << endl;
	cout << "Nom brdf : " << brdf_name << endl;
	cout << "Paramètre puissance spéculaire : " << float_parameters["spec_pow"] << endl;

	TemplateShader* vs = new TemplateShader(ressourceCoreMaterialPath +"Common/BRDF/templates/VS-template.tglsl");
	vs->setIncludes(vs_snippets["includes"]);
	vs->setUniforms(vs_snippets["uniforms"]);
	vs->setInOut(vs_snippets["in_out"]);
	vs->setLayout(vs_snippets["layout"]);
	vs->setMainCode(vs_snippets["main_code"]);
	vs->writeTemplate(ressourceCoreMaterialPath+"BRDFMaterial/Shaders/Main-VS.glsl");

	TemplateShader* fs = new TemplateShader(ressourceCoreMaterialPath+"Common/BRDF/templates/FS-template.tglsl");
	fs->setIncludes(fs_snippets["includes"]);
	fs->setUniforms(fs_snippets["uniforms"]);
	fs->setInOut(fs_snippets["in_out"]);
	fs->setLayout(fs_snippets["layout"]);
	fs->setMainCode(fs_snippets["main_code"]);
	fs->writeTemplate(ressourceCoreMaterialPath+"BRDFMaterial/Shaders/Main-FS.glsl");

	delete(vs);
	delete(fs);

	BRDFMaterial* resultMaterial =  new BRDFMaterial(filename + "Material", "BRDFMaterial");
	
	//création des uniforms avec les paramètres
	for (auto it = vec4_parameters.begin(); it != vec4_parameters.end(); ++it)
		resultMaterial->setUniformVec4(GL_FRAGMENT_SHADER, it->first, it->second);
	for (auto it = float_parameters.begin(); it != float_parameters.end(); ++it)
		resultMaterial->setUniformFloat(GL_FRAGMENT_SHADER, it->first, it->second);
	for (auto it = vec3_parameters.begin(); it != vec3_parameters.end(); ++it)
		resultMaterial->setUniformVec3(GL_VERTEX_SHADER, it->first, it->second);
	for (auto it = bool_parameters.begin(); it != bool_parameters.end(); ++it)
		resultMaterial->setUniformBool(GL_FRAGMENT_SHADER, it->first, it->second);

	return resultMaterial;
}