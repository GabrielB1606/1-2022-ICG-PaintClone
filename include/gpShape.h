#ifndef GP_SHAPE_H
#define GP_SHAPE_H

#include <iostream>

#include "GL/freeglut.h"
#include "imgui.h"

#define ROUNDNUM(x) ((int)( x >= .5f ? (x+1.0f) : x )) 
#define MIN(x, y) ( x >= y ? y : x ) 
#define MAX(x, y) ( x >= y ? x : y ) 
#define ABS(x) ( x < 0 ? -x : x )

using namespace std;

static short vertex_dragging = -1;
static bool center_mode = false;
static bool vertice_mode = false;

class gpShape
{

	protected:
		ImVec4 border_color, fill_color;
		int vertex[3][2];

	public:

		gpShape(int x0, int y0){
			vertex[2][0] = vertex[1][0] = vertex[0][0] = x0;
			vertex[2][1] = vertex[1][1] = vertex[0][1] = y0;
		}

		gpShape(int x0, int y0, int x1, int y1, ImVec4 border_color){
			this->border_color = border_color;
			vertex[2][0] = vertex[0][0] = x0;
			vertex[2][1] = vertex[0][1] = y0;
			vertex[1][0] = x1;
			vertex[1][1] = y1;
		}

		gpShape(ImVec4 border_color){
			this->border_color = border_color;
		}

		~gpShape(){
			// cout << "Se destruyo un shape" << endl;
		}

		int* getCenter(){
			return vertex[2];
		}

		int getCenter(int i){
			return vertex[2][i];
		}

		static void putPixel(int x, int y){
			
			ImGuiIO& io = ImGui::GetIO();

			// y = 30;

			glPointSize(1.5f);
			glBegin(GL_POINTS);
			glVertex2i(x,  (int)io.DisplaySize.y - y);
			// glVertex2i(x, y);
			glEnd();
		}

		void setBorderColor(ImVec4 border_color){
			this->border_color = border_color;
		}

		void setFillColor(ImVec4 fill_color){
			this->fill_color = fill_color;
		}

		virtual void softwareRenderBorder() = 0;
		virtual void softwareRenderFill() = 0;
		
		virtual void hardwareRenderBorder() = 0;
		virtual void hardwareRenderFill() = 0;
		
		void render(bool isHardwareMode){
			glColor4f(fill_color.x * fill_color.w, fill_color.y * fill_color.w, fill_color.z * fill_color.w, fill_color.w);

			if(isHardwareMode)
				hardwareRenderFill();
			else
				softwareRenderFill();

			glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);

			if(isHardwareMode)
				hardwareRenderBorder();
			else
				softwareRenderBorder();
		}

		// recibe el click del mouse y retorna true si efectivamente
		// el objetos fue seleccionado
		// virtual bool onClick(int x, int y);
		
		// // este método es invocado solo hacia el objeto "current"
		// virtual void onMove(int x, int y);

		ImVec4* getBorderColorReference(){
			return &border_color;
		}

		ImVec4* getFillColorReference(){
			return &fill_color;
		}

		// virtual void setVertex(int n, int x, int y) = 0;
		virtual void setVertex(int n, int x, int y){
			vertex[n][0] = x;
			vertex[n][1] = y;

			if( !center_mode ){
				vertex[2][0] = (vertex[0][0]+vertex[1][0]) >> 1;
				vertex[2][1] = (vertex[0][1]+vertex[1][1]) >> 1;
			}else{
				int m = (n+1)&1;
				vertex[m][0] = vertex[2][0] + (vertex[2][0] - vertex[n][0]);
				vertex[m][1] = vertex[2][1] + (vertex[2][1] - vertex[n][1]);
			}
		}

		void setCenter(int x, int y){
			vertex[2][0] = x;
			vertex[2][1] = y;
		}
		

		// podríamos responder a los eventos del mouse
		// . todos responden al click, pero solo uno puede
		// . retornar "yo fui seleccionado"
};

#endif