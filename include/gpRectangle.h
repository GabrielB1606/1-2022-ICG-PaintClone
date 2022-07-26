#ifndef GP_RECT_H
#define GP_RECT_H

#include "gpShape.h"

class gpRectangle : public gpShape{

	private:

	public:
		gpRectangle(int x0, int y0) : gpShape(x0, y0){}

		~gpRectangle(){}

		static void draw(int x0, int y0, int x1, int y1, ImVec4 color){
			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);
			for(int i = x0; i<x1; i++)
				for(int j = y0; j<y1; j++)
					putPixel(i,j);
		}

		void hardwareRenderFill(){
			glBegin(GL_QUADS);
				glVertex2i(vertex[0][0], vertex[0][1]);
				glVertex2i(vertex[1][0], vertex[0][1]);
				glVertex2i(vertex[1][0], vertex[1][1]);
				glVertex2i(vertex[0][0], vertex[1][1]);
			glEnd();
		}

		void hardwareRenderBorder(){
			glBegin(GL_LINE_LOOP);
				glVertex2i(vertex[0][0], vertex[0][1]);
				glVertex2i(vertex[1][0], vertex[0][1]);
				glVertex2i(vertex[1][0], vertex[1][1]);
				glVertex2i(vertex[0][0], vertex[1][1]);
			glEnd();
		}

		void hardwareRender(){
			if(filled){
				glColor4f(fill_color.x * fill_color.w, fill_color.y * fill_color.w, fill_color.z * fill_color.w, fill_color.w);
				hardwareRenderFill();
			}

			glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);
			hardwareRenderBorder();
		}

        void setVertex(int n, int x, int y){

			gpShape::setVertex(n,x,y);

			if( vertex[0][0] > vertex[1][0] ){
				swap( vertex[0], vertex[1] );
				vertex_dragging = (vertex_dragging + 1)&1;
			}

		}

		void softwareRender(){

            int y0, y1;

            if( vertex[0][1] > vertex[1][1] ){
                y0 = vertex[1][1];
                y1 = vertex[0][1];
            }else{
                y1 = vertex[1][1];
                y0 = vertex[0][1];
            }

			glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);
			for(int i = y0; i<=y1; i++){
				putPixel(vertex[0][0], i);
				putPixel(vertex[1][0], i);
			}
			y0++;
			
			for(int i = vertex[0][0]+1; i<vertex[1][0]; i++){
				putPixel(i, vertex[0][1]);
				putPixel(i, vertex[1][1]);
			}

			if(filled){
				glColor4f(fill_color.x * fill_color.w, fill_color.y * fill_color.w, fill_color.z * fill_color.w, fill_color.w);

				for(int i = vertex[0][0]+1; i<vertex[1][0]; i++)
					for(int  j = y0; j<y1; j++)
						putPixel(i,j);
			}
		}

};

#endif