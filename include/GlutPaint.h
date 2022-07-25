#ifndef GLUT_PAINT_H
#define GLUT_PAINT_H

#include <deque>
#include <algorithm>
#include <math.h>

#include "GL/freeglut.h"

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
// static short vertex_dragging = -1;

// drawing modes
static bool hardware_mode = true;
static bool selection_mode = false;

void GlutPaintInit();
void GlutPaintDisplay();
void GlutPaintMouseFunc(int, int, int, int);
void GlutPaintMotionFunc(int, int);
void GlutPaintInstallFuncs();
void GlutPaintCleanup();


void GlutPaintInit(){

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("PaintClone");
 
	glViewport(0, 0, window_width, window_height);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_width, 0, window_height, -1, 1);

}

void GlutPaintDisplay(){

    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    
    glClearColor(background_color.x * background_color.w, background_color.y * background_color.w, background_color.z * background_color.w, background_color.w);
    
    glClear(GL_COLOR_BUFFER_BIT);

    // glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // gpTriangle tr(300, 300);
    // tr.setVertex(1, 700, 300);
    // tr.setVertex(2, 500, 500);
    // tr.setBorderColor( border_color );
    // tr.setFillColor( ImVec4(255, 255, 255, 1) );
    // tr.softwareRender();

    // glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);
    //     glBegin( GL_LINE_STRIP );
    //     glVertex2i(  300, 300 );
    //     glVertex2i(  500, 500 );
    //     glVertex2i(  700, 300 );
    //     glEnd();

    // gpBezier test(300, 300);
    // test.setVertex(1, 500, 500);
    // test.setVertex(2, 700, 300);
    // test.setVertex(3, 800, 700);
    // test.setVertex(4, 900, 715);
    // test.setVertex(5, 1000, 650);
    // test.setVertex(6, 800, 675);
    // test.setVertex(7, 700, 700);
    // test.setVertex(8, 600, 600);
    // test.setVertex(9, 500, 500);
    // test.setBorderColor( border_color );
    // test.setFillColor( fill_color );
    // test.softwareRender();

    // glBegin(GL_TRIANGLES);
    //     glColor3f(2,0,0);
    //     glVertex3f(300, 300, 0);
        
    //     glColor3f(0,1,0);
    //     glVertex3f( 700, 300, 0);
        
    //     glColor3f(0,0,1);
    //     glVertex3f( 500,  500, 0);
    // glEnd();
    glFlush();

    if( current_drawing != nullptr ){
        current_drawing->setFillColor(fill_color);
        current_drawing->setBorderColor(border_color);
    }

    for( gpShape* shape : shapes )
        shape->render(hardware_mode);
    
    if( current_drawing != nullptr )
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
            for(int i = shapes.size()-1; i>=0; i--)
                if( shapes[i]->onClick(x, y) ){
                    
                    if( current_drawing != nullptr )
                        current_drawing->select(false);
                    
                    current_drawing = shapes[i];
                    current_drawing->select(true);
                    shapes.erase( shapes.begin() + i );
                    shapes.emplace_back(current_drawing);
                    std::cout << "yo\n";
                    return;
                }
            if(current_drawing != nullptr){
                current_drawing->select(false);
                current_drawing = nullptr;
            }
        }

        return;
    }

    // Drawing Mode click (not selection)
    if( state == GLUT_DOWN ){
        if( glut_button == GLUT_LEFT_BUTTON ){

            if( DrawBezier == current_shape ){

                if(vertex_dragging == -1){
                    current_drawing = new gpBezier(x, y);
                    vertex_dragging = 1;
                }else if( vertex_dragging < 20 ){
                    current_drawing->setVertex(vertex_dragging, x, y);
                    vertex_dragging++;
                    if( vertex_dragging >= 20 ){
                        shapes.emplace_back(current_drawing);
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
        }

    }else if( state == GLUT_UP ){
        // current_drawing->setVertex(vertex_dragging,x,y);
        if( !vertice_mode && current_shape != DrawBezier ){
            vertex_dragging = -1;
            shapes.emplace_back(current_drawing);
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
    if( vertex_dragging != -1 && current_drawing != nullptr )
        if(  vertice_mode ){
            if( current_shape == DrawTriangle && vertex_dragging < 3 ){
                for(int i = vertex_dragging; i<3; i++)
                    current_drawing->setVertex(i, x, y);
            }

        }else if( current_shape == DrawBezier && vertex_dragging < 20 ){
            current_drawing->setVertex(vertex_dragging, x, y);
            // printf("hey");
        }

}

void GlutPaintInstallFuncs(){
    glutMouseFunc(GlutPaintMouseFunc);
    glutMotionFunc(GlutPaintMotionFunc);
    glutPassiveMotionFunc( GlutPaintPassiveMotionFunc );
}

void GlutPaintCleanup(){

    for(gpShape* shape: shapes)
        delete shape;

}

void deleteCurrent(){

    if( shapes.size() ){
        current_drawing = shapes.back();
        shapes.pop_back();
        delete current_drawing;
        current_drawing = nullptr;
    }

}

#endif