#include <stdio.h>

#include "GL/freeglut.h"

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

// Variable Globales
static int WIDTH = 1280;
static int HEIGHT = 720;

static ImVec4 background_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

static ImVec4 fill_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
static ImVec4 border_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

static bool hardware_mode = true;
static bool center_mode = false;

enum EnumDrawShape{
    DrawLine,
    DrawCircle,
    DrawEllipse,
    DrawRectangle,
    DrawTriangle,
    DrawBezier
};

static int current_shape = 1;
const char* shapes[] = { 
    "Line",
    "Circle",
    "Ellipse",
    "Rectangle",
    "Triangle",
    "Bezier Curve"
};



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

void display_imgui_function(){

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
}

void display_function_glut(void)
{
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

void imgui_glut_display_function(){
     // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    display_imgui_function();

    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    
    glClearColor(background_color.x * background_color.w, background_color.y * background_color.w, background_color.z * background_color.w, background_color.w);
    
    glClear(GL_COLOR_BUFFER_BIT);


    display_function_glut();
    
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Hello World");
 
	glViewport(0, 0, WIDTH, HEIGHT);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);

    glutDisplayFunc(imgui_glut_display_function);

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

    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}