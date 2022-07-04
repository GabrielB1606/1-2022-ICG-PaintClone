#include <stdio.h>

#include "GL/freeglut.h"

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

// #include "GlobalState.h"
// #include "EnumDrawShape.h"
// #include "GlutPaint.h"
#include "ImGuiPaint.h"

// #include "gpShape.hpp"
// #include "gpLine.h"

void display();
void cleanup();

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    GlutPaintInit();

    glutDisplayFunc(display);

    ImGuiPaintInit();

    glutCloseFunc(cleanup);

    // event handler
    GlutPaintInstallFuncs();

    glutMainLoop();

    return 0;
}

void display(){
    
    ImGuiPaintDisplay();
    GlutPaintDisplay();
    
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}

void cleanup(){

    GlutPaintCleanup();

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}