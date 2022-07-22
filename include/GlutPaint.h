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
    glBegin(GL_TRIANGLES);
        glColor3f(1,0,0);
        glVertex3f(100, 100, 0);
        
        glColor3f(0,1,0);
        glVertex3f( 200, 100, 0);
        
        glColor3f(0,0,1);
        glVertex3f( 150,  200, 0);
    glEnd();
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

    if( state == GLUT_DOWN && glut_button == GLUT_LEFT_BUTTON ){

        if(vertice_mode && DrawTriangle == current_shape){
            if(vertex_dragging == -1){
                current_drawing = new gpTriangle(x,y);
                vertex_dragging = 1;
            }else if( vertex_dragging < 3 ){
                current_drawing->setVertex(vertex_dragging, x, y);
                vertex_dragging++;
                if( vertex_dragging >= 3 ){
                    shapes.emplace_back(current_drawing);
                    vertex_dragging = -1;
                }
            }
            
        }else{
            vertex_dragging = 1;
            switch (current_shape){
                case DrawLine:
                    current_drawing = new gpLine(x, y);
                    break;
                case DrawRectangle:
                    current_drawing = new gpRectangle(x,y);
                    break;
                case DrawTriangle:
                    current_drawing = new gpTriangle(x,y);
                    break;
                case DrawCircle:
                    current_drawing = new gpCircle(x,y);
                    break;
                case DrawEllipse:
                    current_drawing = new gpEllipse(x,y);
                    break;

                default:
                    break;
            }
        }
        // if(center_mode)
        //     current_drawing->setCenter(x,y);
        // fill_color = current_drawing->getColorReference();
        
    }else if( state == GLUT_UP ){
        // current_drawing->setVertex(vertex_dragging,x,y);
        if( !vertice_mode ){
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

    if( !vertice_mode && vertex_dragging != -1 ){
        current_drawing->setVertex(vertex_dragging, x, y);
    }
}

void GlutPaintPassiveMotionFunc(int x, int y){

    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)x, (float)y);
    if( io.WantCaptureMouse )
        return;

    if(  vertex_dragging != -1 && vertice_mode){
        if( current_shape == DrawTriangle && current_drawing != nullptr && vertex_dragging < 3 ){
            for(int i = vertex_dragging; i<3; i++)
                current_drawing->setVertex(i, x, y);
        }

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

#endif