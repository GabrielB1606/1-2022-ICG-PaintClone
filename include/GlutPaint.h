#ifndef GLUT_PAINT_H
#define GLUT_PAINT_H

#include <deque>
#include <algorithm>
#include <math.h>
#include <fstream>

#include "GL/freeglut.h"

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

// #include "GlutPaint.h"
#include "tinyfiledialogs.h"

static int window_width = 1280, window_height = 720; 

#include "EnumDrawShape.h"
#include "gpShape.h"
#include "gpLine.h"
#include "gpRectangle.h"
#include "gpTriangle.h"
#include "gpCircle.h"
#include "gpEllipse.h"
#include "gpBezier.h"

// window global variables

static ImVec4 background_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

// drawing global variables
static ImVec4 fill_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
static ImVec4 border_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
static int current_shape = 0;
deque<gpShape*> shapes;
gpShape* current_drawing = nullptr;
int current_drawing_idx = -1;
// static short vertex_dragging = -1;

// drawing modes
static bool hardware_mode = true;

void GlutPaintInit();
void GlutPaintDisplay();
void GlutPaintMouseFunc(int, int, int, int);
void GlutPaintMotionFunc(int, int);
void GlutPaintInstallFuncs();
void GlutPaintCleanup();
void GlutPaintMoveUp();
void GlutPaintMoveDown();
void GlutPaintMoveFront();
void GlutPaintMoveBack();
void GlutPaintDeleteCurrent();
void GlutPaintReshapeFunc(int w, int h);

void TinyFDSaveFile();
void TinyFDLoadFile();


void GlutPaintInit(){

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("PaintClone");
 
	glViewport(0, 0, window_width, window_height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_width, window_height, 0, -1, 1);

}

void GlutPaintDisplay(){

    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    
    glClearColor(background_color.x * background_color.w, background_color.y * background_color.w, background_color.z * background_color.w, background_color.w);
    
    glClear(GL_COLOR_BUFFER_BIT);

    // glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    if( current_drawing != nullptr ){
        current_drawing->setFillColor(fill_color);
        current_drawing->setBorderColor(border_color);
    }

    for( gpShape* shape : shapes )
        shape->render(hardware_mode);
    
    if( current_drawing != nullptr && !current_drawing->isSelected() )
        current_drawing->render(hardware_mode);
}

void GlutPaintMouseFunc(int glut_button, int state, int x, int y){
    
    ImGuiIO& io = ImGui::GetIO();

    io.AddMousePosEvent((float)x, (float)y);
    if ((state == GLUT_DOWN || state == GLUT_UP))
        io.AddMouseButtonEvent(glut_button, state == GLUT_DOWN);

    if( io.WantCaptureMouse )
        return;

    mouse_pos[0] = x;
    mouse_pos[1] = y;

    // Selection Mode click
    if( selection_mode ){

        if( state == GLUT_DOWN && glut_button == GLUT_LEFT_BUTTON ){

            if( current_drawing != nullptr && current_drawing->onVertex(x, y) ){
                current_drawing->updateBoundingBox();
                return;
            }

            for(int i = shapes.size()-1; i>=0; i--)
                if( shapes[i]->onClick(x, y) ){
                    
                    if( current_drawing != nullptr )
                        current_drawing->select(false);
                    
                    current_drawing = shapes[i];
                    current_drawing->select(true);
                    current_drawing_idx = i;
                    filled = current_drawing->isFilled();
                    border_color = current_drawing->getBorderColor();
                    fill_color = current_drawing->getFillColor();
                    // shapes.erase( shapes.begin() + i );
                    // shapes.emplace_back(current_drawing);
                    return;
                }
            if(current_drawing != nullptr){
                current_drawing->select(false);
                current_drawing = nullptr;
            }
        }else if( state == GLUT_UP ){
            if(current_drawing != nullptr)
                current_drawing->updateBoundingBox();
        }

        return;
    }

    // Drawing Mode click (not selection)
    if( state == GLUT_DOWN ){
        if( glut_button == GLUT_LEFT_BUTTON ){

            if( DrawBezier == current_shape ){

                if(vertex_dragging == -1){
                    vertice_mode = false;
                    current_drawing = new gpBezier(x, y);
                    vertex_dragging = 1;
                }else if( vertex_dragging < 20 ){
                    current_drawing->setVertex(vertex_dragging, x, y);
                    vertex_dragging++;
                    if( vertex_dragging >= 20 ){
                        shapes.emplace_back(current_drawing);
                        ((gpBezier*)current_drawing)->setT(n_segments);
                        current_drawing_idx = shapes.size()-1;
                        vertex_dragging = -1;
                    }
                }
                
                return;

            }else if( vertice_mode ){

                if(DrawTriangle == current_shape){
                    if(vertex_dragging == -1){
                        current_drawing = new gpTriangle(x, y);
                        vertex_dragging = 1;
                    }else if( vertex_dragging < 3 ){
                        current_drawing->setVertex(vertex_dragging, x, y);
                        vertex_dragging++;
                        if( vertex_dragging >= 3 ){
                            shapes.emplace_back(current_drawing);
                            current_drawing_idx = shapes.size()-1;
                            vertex_dragging = -1;
                        }
                    }
                }
                
            }else{
                
                vertex_dragging = 1;
                switch (current_shape){
                    case DrawTriangle:
                        current_drawing = new gpTriangle(x,y);
                        break;
                    case DrawLine:
                        vertice_mode = false;
                        current_drawing = new gpLine(x, y);
                        break;
                    case DrawRectangle:
                        vertice_mode = false;
                        current_drawing = new gpRectangle(x,y);
                        break;
                    case DrawCircle:
                        vertice_mode = false;
                        current_drawing = new gpCircle(x,y);
                        break;
                    case DrawEllipse:
                        vertice_mode = false;
                        current_drawing = new gpEllipse(x,y);
                        break;

                    default:
                        break;
                }
            }
            // if(center_mode)
            //     current_drawing->setCenter(x,y);
            // fill_color = current_drawing->getColorReference();

        }else if( glut_button == GLUT_RIGHT_BUTTON && DrawBezier == current_shape ){
            vertex_dragging = -1;
            shapes.emplace_back(current_drawing);
            ((gpBezier*)current_drawing)->setT(n_segments);
            current_drawing_idx = shapes.size()-1;
        }

    }else if( state == GLUT_UP ){
        // current_drawing->setVertex(vertex_dragging,x,y);
        if( current_drawing != nullptr ){
            if( !vertice_mode && current_shape != DrawBezier ){
                vertex_dragging = -1;
                shapes.emplace_back(current_drawing);
                current_drawing_idx = shapes.size()-1;
            }
            
            current_drawing->updateBoundingBox();
            
        }
        
    }
    
}

void GlutPaintMotionFunc(int x, int y){

    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)x, (float)y);
    if( io.WantCaptureMouse )
        return;

    if( current_drawing == nullptr )
        return;

    if( selection_mode ){
        if( current_drawing != nullptr && current_drawing->isSelected() && current_drawing->onVertex(x, y) ){
            // current_drawing->updateBoundingBox();
            return;
        }
        current_drawing->onMove(x, y);
    }

    if( current_shape != DrawBezier && !vertice_mode && vertex_dragging != -1 ){
        current_drawing->setVertex(vertex_dragging, x, y);
    }
    mouse_pos[0] = x;
    mouse_pos[1] = y;
}

void GlutPaintPassiveMotionFunc(int x, int y){

    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)x, (float)y);
    if( io.WantCaptureMouse )
        return;

    if( current_drawing != nullptr ){

        if( current_drawing->isSelected()){
            if(current_drawing->onVertex(x, y, false))
                glutSetCursor(GLUT_CURSOR_INFO);
            else if(current_drawing->onClick(x, y) )
                glutSetCursor(GLUT_CURSOR_CYCLE);
            else
                glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }else{
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }

        if( vertex_dragging != -1 && current_drawing != nullptr )
            if(  vertice_mode ){
                if( current_shape == DrawTriangle && vertex_dragging < 3 ){
                    for(int i = vertex_dragging; i<3; i++)
                        current_drawing->setVertex(i, x, y);
                }

            }else if( current_shape == DrawBezier && vertex_dragging < 20 ){
                current_drawing->setVertex(vertex_dragging, x, y);
            }
    
    }else{
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
}

void GlutPaintReshapeFunc(int w, int h){
    window_width = w;
    window_height = h;
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)w, (float)h);
    glViewport(0, 0, window_width, window_height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_width, h, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // GlutPaintDisplay();
}

void GlutPaintKeyboardFunc(unsigned char c, int x, int y){
    // Send character to imgui
    //printf("char_down_func %d '%c'\n", c, c);
    // ImGuiIO& io = ImGui::GetIO();
    // if (c >= 32)
    //     io.AddInputCharacter((unsigned int)c);

    switch (c){
        case 'x': case 'X':
            GlutPaintCleanup();
            break;
        case 'h': case 'H':
            hardware_mode = !hardware_mode;
            break;
        case 'f': case 'F':
            GlutPaintMoveFront();
            break;
        case 'b': case 'B':
            GlutPaintMoveBack();
            break;
        case '+':
            GlutPaintMoveUp();
            break;
        case '-':
            GlutPaintMoveDown();
            break;
        case 'u': case 'U':
            if( current_drawing != nullptr )
                current_drawing->select(false);
            current_drawing = nullptr;
            break;
        case 127:
            GlutPaintDeleteCurrent();
            break;
        case 's': case 'S':
            TinyFDSaveFile();
            break;
        case 'l': case 'L':
            TinyFDLoadFile();
            break;

        default:
            break;
    }

    (void)x; (void)y; // Unused

}

void GlutPaintInstallFuncs(){
    glutMouseFunc(GlutPaintMouseFunc);
    glutMotionFunc(GlutPaintMotionFunc);
    glutPassiveMotionFunc( GlutPaintPassiveMotionFunc );
    glutReshapeFunc(GlutPaintReshapeFunc);
    glutKeyboardFunc(GlutPaintKeyboardFunc);
}

void GlutPaintCleanup(){

    for(gpShape* shape: shapes)
        delete shape;
    shapes.clear();
    current_drawing = nullptr;

}

void GlutPaintDeleteCurrent(){

    if( current_drawing != nullptr ){
        shapes.erase( shapes.begin() + current_drawing_idx );
        delete current_drawing;
        current_drawing = nullptr;
    }

}

void GlutPaintMoveUp(){
    
    if( current_drawing == nullptr || current_drawing_idx == shapes.size()-1 )
        return;
    
    swap( shapes[ current_drawing_idx ], shapes[ current_drawing_idx+1 ]  );
    current_drawing_idx++;

}

void GlutPaintMoveDown(){
    if( current_drawing == nullptr || current_drawing_idx == 0 )
        return;
    
    swap( shapes[ current_drawing_idx ], shapes[ current_drawing_idx-1 ]  );
    current_drawing_idx--;
}

void GlutPaintMoveFront(){

    if( current_drawing == nullptr || current_drawing_idx == shapes.size()-1 )
        return;
    
    shapes.erase( shapes.begin()+current_drawing_idx );
    shapes.push_back( current_drawing );
    current_drawing_idx = shapes.size()-1;

}

void GlutPaintMoveBack(){

    if( current_drawing == nullptr || current_drawing_idx == 0 )
        return;
    
    shapes.erase( shapes.begin()+current_drawing_idx );
    shapes.push_front( current_drawing );
    current_drawing_idx = 0;

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

    glutSetCursor(GLUT_CURSOR_WAIT);

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
    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
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

#endif