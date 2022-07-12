#ifndef GP_SHAPE_H
#define GP_SHAPE_H

#include <iostream>

#include "GL/freeglut.h"
#include "imgui.h"

#define ROUNDNUM(x) ((int)( x >= .5f ? (x+1.0f) : x )) 
#define MIN(x, y) ( x >= y ? y : x ) 
#define MAX(x, y) ( x >= y ? x : y ) 

using namespace std;

static short vertex_dragging = -1;
static bool center_mode = false;

class gpShape
{
	protected:
		ImVec4 color;
		int vertex[2][2];
		int center[2];

	public:
		gpShape(int x0, int y0){
			ImGuiIO& io = ImGui::GetIO();
			center[0] = vertex[1][0] = vertex[0][0] = x0;
			center[1] = vertex[1][1] = vertex[0][1] = (int)io.DisplaySize.y - y0;
		}

		~gpShape(){
			cout << "Se destruyo un shape" << endl;
		}

		void putPixel(int x, int y){
			glPointSize(1.5f);
			glBegin(GL_POINTS);
			glVertex2i(x, y);
			glEnd();
		}

		void setColor(ImVec4 color){
			this->color = color;
		}

		virtual void softwareRender() = 0;
		virtual void hardwareRender() = 0;
		
		// recibe el click del mouse y retorna true si efectivamente
		// el objetos fue seleccionado
		// virtual bool onClick(int x, int y);
		
		// // este método es invocado solo hacia el objeto "current"
		// virtual void onMove(int x, int y);

		ImVec4* getColorReference(){
			return &color;
		}

		// virtual void setVertex(int n, int x, int y) = 0;
		virtual void setVertex(int n, int x, int y){
			ImGuiIO& io = ImGui::GetIO();
			vertex[n][0] = x;
			vertex[n][1] = (int)io.DisplaySize.y - y;

			if( !center_mode ){
				center[0] = (vertex[0][0]+vertex[1][0]) >> 1;
				center[1] = (vertex[0][1]+vertex[1][1]) >> 1;
			}else{
				int m = (n+1)&1;
				vertex[m][0] = center[0] + (center[0] - vertex[n][0]);
				vertex[m][1] = center[1] + (center[1] - vertex[n][1]);
			}
		}

		void setCenter(int x, int y){
			ImGuiIO& io = ImGui::GetIO();
			center[0] = x;
			center[1] =  (int)io.DisplaySize.y - y;
		}
		

		// podríamos responder a los eventos del mouse
		// . todos responden al click, pero solo uno puede
		// . retornar "yo fui seleccionado"
};

#endif