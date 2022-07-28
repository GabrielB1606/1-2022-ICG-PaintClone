#ifndef GP_SHAPE_H
#define GP_SHAPE_H

#include <iostream>
#include <sstream>

#include "GL/freeglut.h"

using namespace std;

// global variables that affect shapes properties
static short vertex_dragging = -1;
static bool center_mode = false;
static bool vertice_mode = false;
static bool selection_mode = false;
static bool reading_file = false;
static bool filled = true;
static int mouse_pos[2] = {0, 0};
static int n_segments = 50;

class gpShape{

	protected:
		ImVec4 border_color, fill_color;
		int vertex[3][2];
		int bounding_box[2][2];
		bool selected = false;
		bool filled;
		EnumDrawShape shape;

	public:

		EnumDrawShape getShape(){
			return shape;
		}

		virtual std::string toString() = 0;

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

		~gpShape(){}

		int* getCenter(){
			return vertex[2];
		}

		int getCenter(int i){
			return vertex[2][i];
		}

		bool isSelected(){ return selected; }

		static void putPixel(int x, int y){
			
			glPointSize(1.5f);	// for some reason if i don't have this point size my pc won't draw a thing
			glBegin(GL_POINTS);
			glVertex2i(x, y);	
			glEnd();
		}

		void setBorderColor(ImVec4 border_color){
			this->border_color = border_color;
		}

		void setFillColor(ImVec4 fill_color){
			this->fill_color = fill_color;
		}
		
		virtual void hardwareRender() = 0;
		virtual void softwareRender() = 0;
		
		// recibe el click del mouse y retorna true si efectivamente
		// el objetos fue seleccionado
		virtual bool onClick(int x, int y){

			return x >= bounding_box[0][0] && x <= bounding_box[1][0] && y >= bounding_box[0][1] && y <= bounding_box[1][1];

		}
		
		void select(bool s){
			selected = s;
		}

		virtual void updateBoundingBox(){
			bounding_box[0][0] = MIN(vertex[0][0], vertex[1][0]);
			bounding_box[0][1] = MIN(vertex[0][1], vertex[1][1]);
			bounding_box[1][0] = MAX(vertex[0][0], vertex[1][0]);
			bounding_box[1][1] = MAX(vertex[0][1], vertex[1][1]);

			vertex[0][0] = bounding_box[0][0];
			vertex[0][1] = bounding_box[0][1];
			vertex[1][0] = bounding_box[1][0];
			vertex[1][1] = bounding_box[1][1];

			// bounding_box[0][0] -= 3;
			// bounding_box[0][1] -= 3;
			// bounding_box[1][0] += 3;
			// bounding_box[1][1] += 3;

		}

		static void draw7x7(int h, int k){
			for(int x = -3; x<=3; x++){
				glColor3f(70, 70, 70);
				for(int y = -3; y<=3; y++)
					putPixel( h+x, k+y );
				glColor3f(0, 0, 0);
				putPixel( h+x, k+x );
				putPixel( h+x, k-x );
			}
		}

		virtual void renderBoundingBox(){

			draw7x7(vertex[0][0], vertex[0][1]);
			draw7x7(vertex[1][0], vertex[1][1]);
			draw7x7(vertex[0][0], vertex[1][1]);
			draw7x7(vertex[1][0], vertex[0][1]);

		}

		void render(bool isHardwareMode){

			if(isHardwareMode)
				hardwareRender();
			else
				softwareRender();

			if( selected )
				renderBoundingBox();

		}

		
		// // este método es invocado solo hacia el objeto "current"
		// virtual void onMove(int x, int y);

		ImVec4 getBorderColor(){
			return border_color;
		}

		ImVec4 getFillColor(){
			return fill_color;
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

			// updateBoundingBox();
		}

		void setCenter(int x, int y){
			vertex[2][0] = x;
			vertex[2][1] = y;
		}
		
		virtual bool onVertex(int x, int y, bool move = true){
			int dx, dy;
			// int diff[2] = {x - mouse_pos[0], y - mouse_pos[1]};

			dx = vertex[0][0] - x;
			if( (ABS(dx)) <= 8 ){
				dy = vertex[0][1] - y;
				if( (ABS(dy)) <= 8){
					if(move){
						vertex[0][0] = x;
						vertex[0][1] = y;
					}
					return true;
				}
				dy = vertex[1][1] - y;
				if( (ABS(dy)) <= 8){
					if(move){
						vertex[0][0] = x;
						vertex[1][1] = y;
					}
					return true;
				}
			}

			dx = vertex[1][0] - x;
			if( (ABS(dx)) <= 8 ){
				dy = vertex[0][1] - y;
				if( (ABS(dy)) <= 8){
					if(move){
						vertex[1][0] = x;
						vertex[0][1] = y;
					}
					return true;
				}
				dy = vertex[1][1] - y;
				if( (ABS(dy)) <= 8){
					if(move){
						vertex[1][0] = x;
						vertex[1][1] = y;
					}
					return true;
				}
			}

			return false;
			
		}

		virtual void onMove(int x, int y){
			
			if(onVertex(x, y)){
				// updateBoundingBox();
				return;
			}

			if( onClick(x, y) && selected ){
				
				int diff[2] = {x - mouse_pos[0], y - mouse_pos[1]};

				for(int i =0; i<3; i++){
					vertex[i][0] += diff[0];
					vertex[i][1] += diff[1];
				}
				for(int i = 0; i< 2; i++){
					bounding_box[i][0] += diff[0];
					bounding_box[i][1] += diff[1];
				}
			}

		}

		bool isFilled(){
			return filled;
		}

		void setFilled(bool f){
			filled = f;
		}

		
};

#endif