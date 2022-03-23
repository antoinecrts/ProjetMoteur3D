#include "ColorMaterial.h"
#include "Engine/Base/Node.h"
#include "Engine/Base/Scene.h"


ColorMaterial::ColorMaterial(std::string name, const glm::vec4 & c):
	MaterialGL(name,"ColorMaterial")
{
	
	modelM = vp->uniforms()->getGPUmat4("Model");
	modelViewProj = vp->uniforms()->getGPUmat4("MVP");
	color = fp->uniforms()->getGPUvec4("CPU_color");
	color->Set(c);
}
ColorMaterial::~ColorMaterial()
{

}

void ColorMaterial::setColor(glm::vec4 c)
{
	color->Set(c);
}

void ColorMaterial::render(Node *o)
{
	
	if (m_ProgramPipeline)
	{		
		m_ProgramPipeline->bind();
		o->drawGeometry(GL_TRIANGLES);
		m_ProgramPipeline->release();
	}
}

void ColorMaterial::update(Node* o,const int elapsedTime)
{
	
	if (o->frame()->updateNeeded())
	{
		modelM->Set(o->frame()->getRootMatrix());
	} 
	
	
}

void ColorMaterial::displayInterface()
{
	ImGui::Text("%s",m_Name.c_str());
	ImGui::Separator();
	ImGui::Text("Color :");
	ImGui::SameLine();
	glm::vec4 c = color->getValue();

	ImVec4 color = ImColor(c.x, c.y, c.z, c.w);
	if (ImGui::ColorButton(color))
		ImGui::OpenPopup("MaterialColor");
	if (ImGui::BeginPopup("MaterialColor"))
	{
		ImGui::ColorPicker4("", (float*)&color);
		ImGui::EndPopup();
		setColor(glm::vec4(color.x, color.y, color.z, color.w));
	}
	
}