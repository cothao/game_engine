#ifndef GUI_LOG_H
#define GUI_LOG_H
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "engine.h"
#include "gui.h"
#include <vector>


std::vector<std::string> logs;


void displayLog(bool editMode)
{

	if (editMode)
	{
	if (ImGui::Begin("Log", NULL, ImGuiWindowFlags_NoCollapse))
	{

		for (std::string log: logs)
		{
			ImGui::Text(log.c_str());
		}

		ImGui::End();
	}

	}

}

#endif // !GUI_LOG_H
