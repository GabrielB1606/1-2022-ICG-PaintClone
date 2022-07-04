#ifndef GP_SHAPE_H
#define GP_SHAPE_H

#include <iostream>

#include "GL/freeglut.h"
#include "imgui.h"

#include "GlobalState.h"

using namespace std;


class gpShape
{
	protected:
		ImVec4 color;
		int vertex[2][2];

	public:
		gpShape(int x0, int y0){
			vertex[0][0] = x0;
			vertex[0][1] = window_height - y0;
			vertex[1][0] = x0;
			vertex[1][1] = window_height - y0;
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

		virtual void render() = 0;
		
		// recibe el click del mouse y retorna true si efectivamente
		// el objetos fue seleccionado
		// virtual bool onClick(int x, int y);
		
		// // este método es invocado solo hacia el objeto "current"
		// virtual void onMove(int x, int y);

		ImVec4* getColorReference(){
			return &color;
		}

		void setVertex(int n, int x, int y){
			vertex[n][0] = x;
			vertex[n][1] = window_height - y;
		}

		// podríamos responder a los eventos del mouse
		// . todos responden al click, pero solo uno puede
		// . retornar "yo fui seleccionado"
};

#endif