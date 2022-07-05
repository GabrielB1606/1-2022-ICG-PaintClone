#ifndef GP_SHAPE_H
#define GP_SHAPE_H

#include <iostream>

#include "GL/freeglut.h"
#include "imgui.h"

using namespace std;


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

		void setVertex(int n, int x, int y){
			ImGuiIO& io = ImGui::GetIO();
			vertex[n][0] = x;
			vertex[n][1] = (int)io.DisplaySize.y - y;

			center[0] = (vertex[0][0]+vertex[1][0]) >> 1;
			center[1] = (vertex[0][1]+vertex[1][1]) >> 1;

		}

		void setVertexCenterMode(int n, int x, int y){
			ImGuiIO& io = ImGui::GetIO();
			vertex[n][0] = x;
			vertex[n][1] = (int)io.DisplaySize.y - y;

			int m = (n+1)&1;
			vertex[m][0] = center[0] + (center[0] - vertex[n][0]);
			vertex[m][1] = center[1] + (center[1] - vertex[n][1]);
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