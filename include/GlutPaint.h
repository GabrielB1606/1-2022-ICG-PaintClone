#ifndef GLUT_PAINT_H
#define GLUT_PAINT_H

#include <deque>

#include "GL/freeglut.h"

#include "EnumDrawShape.h"
#include "gpShape.hpp"
#include "gpLine.h"

// window global variables

static ImVec4 background_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

// drawing global variables
static ImVec4 fill_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
static ImVec4 border_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
static int current_shape = 0;
deque<gpShape*> shapes;
gpShape* current_drawing = nullptr;
static short vertex_dragging = -1;

// drawing modes
static bool hardware_mode = true;
static bool alt_drawing_mode = false;

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

    for( gpShape* shape : shapes )
        shape->render();

    if( current_drawing != nullptr )
        current_drawing->render();

}

void GlutPaintMouseFunc(int glut_button, int state, int x, int y){
    
    ImGuiIO& io = ImGui::GetIO();

    io.AddMousePosEvent((float)x, (float)y);
    if ((state == GLUT_DOWN || state == GLUT_UP))
        io.AddMouseButtonEvent(glut_button, state == GLUT_DOWN);

    if( io.WantCaptureMouse )
        return;

    if( state == GLUT_DOWN && glut_button == GLUT_LEFT_BUTTON ){
        vertex_dragging = 1;
        switch (current_shape){
            case DrawLine:
                current_drawing = new gpLine(x, y);
                break;
            
            default:
                break;
        }
        current_drawing->setColor(fill_color);
        // fill_color = current_drawing->getColorReference();
        
    }else if( state == GLUT_UP ){
        current_drawing->setVertex(vertex_dragging,x,y);
        vertex_dragging = -1;
        shapes.emplace_back(current_drawing);
    }
    
}

void GlutPaintMotionFunc(int x, int y){

    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)x, (float)y);

    if( vertex_dragging != -1 ){
        current_drawing->setVertex(vertex_dragging, x, y);
    }
}

void GlutPaintInstallFuncs(){
    glutMouseFunc(GlutPaintMouseFunc);
    glutMotionFunc(GlutPaintMotionFunc);
}

void GlutPaintCleanup(){

    for(gpShape* shape: shapes)
        delete shape;

}

#endif