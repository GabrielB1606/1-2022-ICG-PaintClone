#ifndef IMGUI_PAINT_H
#define IMGUI_PAINT_H

#include "GlutPaint.h"


void ImGuiPaintInit(){
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

}


// yanked code from the ImGui demo, (?) icon that shows text when hover
static void HelpMarker(const char* desc){

    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()){
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

}

// display function for ImGui menu
void ImGuiPaintDisplay(){

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    // Setup the actual GUI
    ImGui::Begin("Menu");
        ImVec2 size = ImVec2(200, 700);

        // File management buttons
        if(ImGui::Button("Load File"))
            TinyFDLoadFile();
        ImGui::SameLine();
        if(ImGui::Button("Save in File"))
            TinyFDSaveFile();

        // General scene config
        ImGui::Text("Background Color");
        ImGui::ColorEdit3(" ", (float*)&background_color); // Edit 3 floats representing a color
        ImGui::Checkbox("Hardware Mode", &hardware_mode);

        // General tools to modify shapes position
        ImGui::Separator();
        ImGui::Text("Tools");
        ImGui::Checkbox("Selection Mode", &selection_mode);
        ImGui::SameLine();
        HelpMarker("Click a shape to change it's properties.");

        if(ImGui::Button("Delete"))
            GlutPaintDeleteCurrent();
        ImGui::SameLine();
        if(ImGui::Button("Clean Screen"))
            GlutPaintCleanup();

        if(ImGui::Button("Move Up"))
            GlutPaintMoveUp();
        ImGui::SameLine();
        if(ImGui::Button("Move Down"))
            GlutPaintMoveDown();
        
        if(ImGui::Button("Move Front"))
            GlutPaintMoveFront();
        ImGui::SameLine();
        if(ImGui::Button("Move Back"))
            GlutPaintMoveBack();

        ImGui::Separator();
        // Select shape to draw
        ImGui::Text("Shape");
        const char* shapes[] = { 
            "Line",
            "Circle",
            "Ellipse",
            "Rectangle",
            "Triangle",
            "Bezier Curve"
        };
        if(ImGui::ListBox(" ", &current_shape, shapes, IM_ARRAYSIZE(shapes), 6)){
            selection_mode = false;
            if( current_drawing != nullptr ){
                current_drawing->select(false);
                current_drawing = nullptr;
            }
            filled = true;
        }

        ImGui::Separator();
        // Modify selected shape properties
        ImGui::Text("Properties");

        if( (current_shape != DrawBezier && current_shape != DrawTriangle && current_drawing == nullptr) || ( current_drawing != nullptr && current_drawing->getShape() != DrawBezier && current_drawing->getShape() != DrawTriangle) ){
            ImGui::Checkbox("Center Mode", &center_mode);
            ImGui::SameLine();
            HelpMarker("The first point given will be taken as the center.");
        }

        if(current_drawing != nullptr){

            if( current_drawing->getShape() == DrawBezier ){
                
                if(ImGui::SliderInt("Number of\nSegments", &n_segments, 1, 100) )            // Edit 1 float using a slider from 0.0f to 1.0f
                    ((gpBezier*)current_drawing)->setT(n_segments);
                ImGui::SameLine();
                HelpMarker("Number of divisions in the curve. A bigger quantity implies a smoother curve with a penalty in performance.");
            }

            if( current_drawing->getShape() == DrawTriangle ){
                ImGui::Checkbox("Vertice Mode", &vertice_mode);
                ImGui::SameLine();
                HelpMarker("Click three times to define the position of each vertex.");
            }

            ImGui::Text("Colors");

            ImGui::ColorEdit3("Border", (float*)&border_color); // Edit 3 floats representing a color
            
            if( current_shape != DrawBezier && current_shape != DrawLine ){
                if(ImGui::Checkbox("Fill Figure", &filled))
                    current_drawing->setFilled(filled);
                
                if(filled)
                    ImGui::ColorEdit3("Fill", (float*)&fill_color); // Edit 3 floats representing a color
            }
        }
        
    ImGui::End();

    // Rendering
    ImGui::Render();

}

#endif