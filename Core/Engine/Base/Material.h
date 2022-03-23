#ifndef _MATERIAL_
#define _MATERIAL_
#include <string>

class Node;
class Material
{
	public:
		Material();
		Material(std::string name,std::string className = "Unknown");
		~Material();

		virtual void render(Node *o);
		virtual void update(Node *o,const int elapsedTime);

		const std::string getName();


		bool valid;
		bool show_interface;
		virtual void displayInterface();
	protected:
		std::string m_Name;
		std::string m_ClassName;

};



#endif
