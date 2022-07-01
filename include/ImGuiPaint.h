#pragma once

#include "GlutPaint.h"

static int current_shape = 1;
const char* shapes[] = { 
    "Line",
    "Circle",
    "Ellipse",
    "Rectangle",
    "Triangle",
    "Bezier Curve"
};

void ImGuiPaintInit(){
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    // FIXME: Consider reworking this example to install our own GLUT funcs + forward calls ImGui_ImplGLUT_XXX ones, instead of using ImGui_ImplGLUT_InstallFuncs().
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();
}

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void ImGuiPaintDisplay(){

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    // Setup the actual GUI
    ImGui::Begin("Hello World");
        ImVec2 size = ImGui::GetItemRectSize();

        ImGui::Text("Background Color");
        ImGui::ColorEdit3(" ", (float*)&background_color); // Edit 3 floats representing a color

        ImGui::Checkbox("Hardware Mode", &hardware_mode);

        ImGui::Separator();
        ImGui::Text("Draw");

        switch (current_shape)
        {
        case DrawEllipse:
        case DrawCircle:
            ImGui::Checkbox("Center Mode", &center_mode);
            ImGui::SameLine();
            HelpMarker("The first point given will be taken as the center.\n\n(Drag while pressing Alt)");
            break;
        case DrawTriangle:
            ImGui::Checkbox("Vertice Mode", &vertice_mode);
            ImGui::SameLine();
            HelpMarker("Click three times to define the position of each vertex.");
            break;
        
        default:
            break;
        }

        ImGui::Text("Shape");
        ImGui::SameLine();
        HelpMarker("Ayuda");
        
        ImGui::ListBox(" ", &current_shape, shapes, IM_ARRAYSIZE(shapes), 6);

        ImGui::Text("Colors");
        ImGui::ColorEdit3("Fill", (float*)&fill_color); // Edit 3 floats representing a color
        ImGui::ColorEdit3("Border", (float*)&border_color); // Edit 3 floats representing a color
        
    ImGui::End();

    // Rendering
    ImGui::Render();

}