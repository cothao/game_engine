#include "gui.h"
#include "shader.h"

template<typename ValueType>
std::string stringulate(ValueType v)
{
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

Gui::Gui()
{

}

void Gui::displayGui(Engine& engine)
{

    if (ImGui::Begin("Hello world"))
    {

        if (ImGui::BeginTabBar("MenuItems"))
        {

            if (ImGui::BeginTabItem("Create Object"))
            {
                if (ImGui::BeginTabBar("Create Object"))
                {
                    const char* items[] = { "Cube", "Light", "Plane" };
                    static const char* current_item = "Cube";

                    if (ImGui::BeginCombo(current_item, *items))
                    {
                        for (int i = 0; i < IM_ARRAYSIZE(items); i++)
                        {
                            bool is_selected = (current_item == items[i]);
                            if (ImGui::Selectable(items[i], is_selected))
                            {
                                current_item = items[i];
                            }
                            if (is_selected)
                            {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::InputFloat3("Position", this->Position);

                    if (ImGui::Button("Create"))
                    {
                        if (current_item == "Cube")
                        {
                            Shader pShader("./pShader.vs", "./pShader.fs");
                            engine.cubes.push_back(*new Cube(pShader, glm::vec3(1.0), glm::vec3(this->Position[0], this->Position[1], this->Position[2]), 0.0, glm::vec3(0., 1., 0.), glm::vec3(0.8, 0.8, 0.8)));
                            std::cout << "Cubes:" << sizeof(engine.cubes) << "\n";
                        }
                    }

                    ImGui::EndTabBar();
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Objects"))
            {
                if (ImGui::BeginTabBar("Objects"))
                {
                    if (ImGui::BeginTabItem("Cube"))
                    {
                        int cubeIndex = 0;

                        for (Cube &cube : engine.cubes)
                        {

                            std::string labelX = "X##" + std::to_string(cubeIndex);
                            std::string labelY = "Y##" + std::to_string(cubeIndex);
                            std::string labelZ = "Z##" + std::to_string(cubeIndex);

                            ImGui::InputFloat(labelX.c_str(), &cube.Position.x);
                            ImGui::InputFloat(labelY.c_str(), &cube.Position.y);
                            ImGui::InputFloat(labelZ.c_str(), &cube.Position.z);
                            cubeIndex++;
                            ImGui::Separator();
                        }
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Lights"))
                    {

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Planes"))
                    {

                        ImGui::EndTabItem();
                    }


                    ImGui::EndTabBar();
                }
                ImGui::EndTabItem();

            }

            ImGui::EndTabBar();
        }





        ImGui::End();

        ImGui::Render();
    }
}

void Gui::initGui(Engine engine)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(engine.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Gui::newGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}