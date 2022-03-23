#include "ImGuiLogger.h"

Logger::Logger()
{
	disp_error = true;
	disp_warning = true;
	disp_info = true;
	disp_trace = false;
	show_interface = false;
	current_type = Comm_Type::TRACE;
};
void Logger::Clear()
{
	Buf.clear();
	loggedMsg.clear();
}
Logger & Logger::operator << (const char* value)
{
	Buf.append("%s",value);
	if (current_type != Comm_Type::TRACE)
		std::cout << value;
	if (std::string(value).compare("[WARNING]") == 0)
		current_type = Comm_Type::WARNING;
	else if (std::string(value).compare("[INFO]")== 0)
		current_type = Comm_Type::INFO;
	else if (std::string(value).compare("[ERROR]")== 0)
		current_type = Comm_Type::ERROR;
	else if (std::string(value).compare("[TRACE]")== 0)
		current_type = Comm_Type::TRACE;
	return *this;
}

Logger &Logger::operator <<(const std::string value)
{
	Buf.append("%s",value.c_str());
	if (current_type != Comm_Type::TRACE)
		std::cout << value;
	return *this;
}

Logger &Logger::operator <<(std::ostream& (*os)(std::ostream&))
{
	if (current_type != Comm_Type::TRACE)
		std::cout << std::endl;
	std::string c = Buf.c_str();
	float time = ImGui::GetTime();
	loggedMsg.push_back(_msg(current_type, c));
	Buf.clear();
	current_type = Comm_Type::TRACE;
	ScrollToBottom = true;
	return *this;
}
void Logger::Draw(const char* title)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);

	if (!ImGui::Begin(title, &show_interface))
	{
		ImGui::End();
		return;
	}
	if (ImGui::Button("Clear"))
		Clear();


	ImGui::SameLine();
	if (disp_error)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1.0f, 0.5f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1.0f, 0.8f, 0.8f));
		if (ImGui::Button("Error"))
			disp_error = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		if (ImGui::Button("Error"))
			disp_error = true;
	}
	ImGui::SameLine();
	if (disp_warning)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.15f, 0.5f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.15f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.15f, 0.8f, 0.8f));
		if (ImGui::Button("Warning"))
			disp_warning = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		if (ImGui::Button("Warning"))
			disp_warning = true;
	}
	ImGui::SameLine();
	if (disp_info)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1.0f, 0.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1.0f, 0.0f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1.0f, 0.0f, 0.8f));
		if (ImGui::Button("Info"))
			disp_info = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		if (ImGui::Button("Info"))
			disp_info = true;
	}
	ImGui::SameLine();
	if (disp_trace)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.5f, 0.5f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.5f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.5f, 0.5f, 0.6f));
		if (ImGui::Button("Trace"))
			disp_trace = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		if (ImGui::Button("Trace"))
			disp_trace = true;
	}
	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (unsigned int i = 0; i < loggedMsg.size(); i++)
	{
		switch (loggedMsg[i].m_Type)
		{
		case Comm_Type::INFO:
			if (disp_info)
				loggedMsg[i].print_to_log(ImColor::HSV(1.0f, 0.0f, 0.7f));
			break;
		case Comm_Type::TRACE:
			if (disp_trace)
				loggedMsg[i].print_to_log(ImColor::HSV(0.5f, 0.6f, 0.5f));
			break;
		case Comm_Type::WARNING:
			if (disp_warning)
				loggedMsg[i].print_to_log(ImColor::HSV(0.15f, 0.6f, 0.7f));
			break;
		case Comm_Type::ERROR:
			if (disp_error)
				loggedMsg[i].print_to_log(ImColor::HSV(1.0f, 0.7f, 0.8f));
			break;
		}
	}
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::EndChild();
	ImGui::End();
}