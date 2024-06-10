#ifndef GUI_LOG_H
#define GUI_LOG_H
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "engine.h"
#include "gui.h"

void displayLog(bool editMode)
{

	if (editMode)
	{
	if (ImGui::Begin("Log", NULL, ImGuiWindowFlags_NoCollapse))
	{
		ImGui::End();
	}

	}

}

#endif // !GUI_LOG_H
