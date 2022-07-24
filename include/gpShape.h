#ifndef GP_SHAPE_H
#define GP_SHAPE_H

#include <iostream>

#include "GL/freeglut.h"
#include "imgui.h"

#define ROUNDNUM(x) ((int)( x >= .5f ? (x+1.0f) : x )) 
#define MIN(x, y) ( x >= y ? y : x ) 
#define MAX(x, y) ( x >= y ? x : y ) 
#define ABS(x) ( x < 0 ? -x : x )
#define MOD_INC(x, y) ( x+1 == y ? 0 : x+1 )
#define MOD_DEC(x, y) ( x-1 < 0 ? y-1 : x-1 )

using namespace std;

static short vertex_dragging = -1;
static bool center_mode = false;
static bool vertice_mode = false;

class gpShape
{

	protected:
		ImVec4 border_color, fill_color;
		int vertex[3][2];
		int bounding_box[2][2];
		bool selected = false;

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
		
		virtual void hardwareRender() = 0;
		virtual void softwareRender() = 0;
		
		// recibe el click del mouse y retorna true si efectivamente
		// el objetos fue seleccionado
		virtual bool onClick(int x, int y) = 0;
		
		void select(bool s){
			bounding_box[0][0] = MIN(vertex[0][0], vertex[1][0]);
			bounding_box[0][1] = MIN(vertex[0][1], vertex[1][1]);
			bounding_box[1][0] = MAX(vertex[0][0], vertex[1][0]);
			bounding_box[1][1] = MAX(vertex[0][1], vertex[1][1]);
			
			selected = s;
		}

		virtual void renderBoundingBox(){

			glColor3f(70, 70, 70);
			for(int i = 0; i<2; i++)
				for(int j = 0; j<2; j++)
					for(int x = -3; x<=3; x++)
						for(int y = -3; y<=3; y++)
							putPixel( bounding_box[i][0]+x, bounding_box[j][1]+y );

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
		
		float Q_rsqrt( float number ){
			long i;
			float x2, y;
			const float threehalfs = 1.5F;

			x2 = number * 0.5F;
			y = number;
			i = *(long*) &y;					//	evil floating point bit hack
			i = 0x5f3759df - (i>>1);			//	what the fuck?
			y = *(float *) &i;
			y = y * (threehalfs - (x2*y*y));	//	1st iteration
			y = y * (threehalfs - (x2*y*y));	//	2nd iteration, can be removed

			return y;

		}

		// podríamos responder a los eventos del mouse
		// . todos responden al click, pero solo uno puede
		// . retornar "yo fui seleccionado"
};

#endif