#pragma once

#include "GL/freeglut.h"

static int window_width = 1280, window_height = 720; 

static ImVec4 background_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
static ImVec4 fill_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
static ImVec4 border_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

static bool hardware_mode = true;
static bool center_mode = false;
static bool vertice_mode = false;



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

}