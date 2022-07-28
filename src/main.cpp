#include <stdio.h>

#include "GL/freeglut.h"

// macros n stuff
#define ROUNDNUM(x) ((int)( (x) >= .5f ? ((x)+1.0f) : (x) )) // cast float -> int rounding
#define MIN(x, y) ( (x) >= (y) ? (y) : (x) )
#define MAX(x, y) ( (x) >= (y) ? (x) : (y) ) 
#define ABS(x) ( (x) < 0 ? -1*(x) : (x) ) // get absolute value
#define MOD_INC(x, y) ( ((x)+1) == (y) ? 0 : (x)+1 ) // (x+1)%y
#define MOD_DEC(x, y) ( ((x)-1) < 0 ? (y)-1 : (x)-1 ) // if (x-1)<0, returns y-1 

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

#include "ImGuiPaint.h"

void display();
void cleanup();

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    //  initialize paint things 
    GlutPaintInit();

    glutDisplayFunc(display);

    // initialize ImGui things
    ImGuiPaintInit();

    // call clean up functions before closing
    glutCloseFunc(cleanup);

    // event handlers
    GlutPaintInstallFuncs();

    glutMainLoop();

    return 0;
}

void display(){
    
    // Display ImGui
    ImGuiPaintDisplay();

    // Display Paint
    GlutPaintDisplay();
    
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}

void cleanup(){

    // Paint clean up
    GlutPaintCleanup();

    // ImGui clean up
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}