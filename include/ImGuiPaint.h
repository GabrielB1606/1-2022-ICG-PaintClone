#ifndef IMGUI_PAINT_H
#define IMGUI_PAINT_H

#include "GlutPaint.h"
#include "tinyfiledialogs.h"
#include <fstream>


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

void ImGuiPaintMotionFunc(int x, int y){
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)x, (float)y);
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

char const * lFilterPatterns[1] = { "*.txt"};

void TinyFDLoadFile(){
    char* filename = tinyfd_openFileDialog(
		"Load File",
		"",
		1,
		lFilterPatterns,
		NULL,
		0);
    
    if( !filename ){
        tinyfd_messageBox(
			"Error",
			"Couldn't open file",
			"ok",
			"error",
			1);
        return;
    }

    ifstream f( filename );
    if(!f){
       tinyfd_messageBox(
			"Error",
			"Couldn't open file",
			"ok",
			"error",
			1);
        return;
    }

    reading_file = true;

    std::string str;

    GlutPaintCleanup();

    f >> str;
    if( str.compare("BACKGROUND") == 0 ){
        f >> background_color.x >> background_color.y >> background_color.z;
        f >> str;
    }

    int x, y, n;
    float r, g, b;

    while( !f.eof() ){
        
        f >> x >> y;
        if( str.find("LINE") != std::string::npos )
            current_drawing = new gpLine(x, y);

        else if( str.find("BEZIER") != std::string::npos )
           current_drawing = new gpBezier(x, y);

        else if( str.find("CIRCLE") != std::string::npos )
           current_drawing = new gpCircle(x, y);

        else if( str.find("ELLIPSE") != std::string::npos )
           current_drawing = new gpEllipse(x, y);

        else if( str.find("TRIANGLE") != std::string::npos ){
            current_drawing = new gpTriangle(x, y);
            f >> x >> y;
            current_drawing->setVertex(2, x, y);
        }

        else if( str.find("RECTANGLE") != std::string::npos )
           current_drawing = new gpRectangle(x, y);
        
        current_drawing->setFilled( str.find("FILLED") != std::string::npos );

        if( current_drawing->getShape() == DrawBezier ){

            n = atoi( str.substr(6, str.size() ).c_str() );
            for(int i = 1; i<n; i++){
                f >> x >> y;
                current_drawing->setVertex(i, x, y);
            }

            ((gpBezier*)current_drawing)->updateSegments();

        }else{
            f >> x >> y;
            current_drawing->setVertex(1, x, y);
        }

        f >> r >> g>> b;
        current_drawing->setBorderColor( ImVec4(r, g, b, 1) );

        if( current_drawing->isFilled() ){
            f >> r >> g>> b;
            current_drawing->setFillColor( ImVec4(r, g, b, 1) );
        }

        current_drawing->updateBoundingBox();
        shapes.push_back(current_drawing);
        
        // std::cout << "loading... " << current_drawing->getShape() << "\n";

        current_drawing = nullptr;

        if( !f.eof() )
            f >> str;

    }
    // std::cout << "final de carga\n";
    reading_file = false;

}

void TinyFDSaveFile(){
    char* filename = tinyfd_saveFileDialog(
		"Save Current State",
		"GlutPaintFile.txt",
		1,
		lFilterPatterns,
		NULL);

    if( !filename ){
        tinyfd_messageBox(
			"Error",
			"Couldn't open file",
			"ok",
			"error",
			1);
        return;
    }
    
    std::stringstream txt;

    txt << "BACKGROUND "<<background_color.x << " " << background_color.y << " " << background_color.z << "\n";

    for( size_t i = 0; i < shapes.size(); i++ )
        txt << shapes[i]->toString() << "\n";
    
    ofstream f( filename );

    if(f.fail() ){
        tinyfd_messageBox(
			"Error",
			"Couldn't open file",
			"ok",
			"error",
			1);
        return;
    }

    f << txt.str();
    f.close();

}

void ImGuiPaintDisplay(){

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    // Setup the actual GUI
    ImGui::Begin("Menu");
        ImVec2 size = ImGui::GetItemRectSize();

        if(ImGui::Button("Load File"))
            TinyFDLoadFile();
        ImGui::SameLine();
        if(ImGui::Button("Save in File"))
            TinyFDSaveFile();

        ImGui::Text("Background Color");
        ImGui::ColorEdit3(" ", (float*)&background_color); // Edit 3 floats representing a color

        ImGui::Checkbox("Hardware Mode", &hardware_mode);

        ImGui::Separator();
        ImGui::Text("Tools");
        ImGui::Checkbox("Selection Mode", &selection_mode);

        if(ImGui::Button("Delete"))
            GlutPaintDeleteCurrent();

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
        // ImGui::Text("Draw");

        ImGui::Text("Shape");
        ImGui::SameLine();
        HelpMarker("Ayuda");
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
        ImGui::Text("Properties");

        if( current_shape != DrawBezier ){
            ImGui::Checkbox("Center Mode", &center_mode);
            ImGui::SameLine();
            HelpMarker("The first point given will be taken as the center.\n\n(Drag while pressing Alt)");
        }

        if(current_drawing != nullptr){

            if( current_drawing->getShape() == DrawBezier ){
                
                if(ImGui::SliderInt("Number of\nSegments", &n_segments, 1, 150) )            // Edit 1 float using a slider from 0.0f to 1.0f
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