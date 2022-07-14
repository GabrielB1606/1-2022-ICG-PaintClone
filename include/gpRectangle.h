#ifndef GP_RECT_H
#define GP_RECT_H

#include "gpShape.h"

class gpRectangle : public gpShape{

	private:

	public:
		gpRectangle(int x0, int y0) : gpShape(x0, y0){
			
	
		}

		~gpRectangle()
		{
			cout << "Se destruyo una linea" << endl;
		}

		// void set(int x0, int y0, int x1, int y1)
		// {
		// 	this->vertex[0][0] = x0;
		// 	this->y0 = y0;
		// 	this->x1 = x1;
		// 	this->y1 = y1;
		// }

		void hardwareRender()
		{
			// despliegas la línea con el algoritmo de bresenham
			// setColor(color[0], color[1], color[2]);
			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

			// user putpixel de aquí en adelante... con Bresenham
			glBegin(GL_QUADS);
				glVertex2i(vertex[0][0], window_height -vertex[0][1]);
				glVertex2i(vertex[1][0], window_height -vertex[0][1]);
				glVertex2i(vertex[1][0], window_height -vertex[1][1]);
				glVertex2i(vertex[0][0], window_height -vertex[1][1]);
			glEnd();
		}

        void setVertex(int n, int x, int y){

			gpShape::setVertex(n,x,y);

			if( vertex[0][0] > vertex[1][0] ){
				swap( vertex[0], vertex[1] );
				vertex_dragging = (vertex_dragging + 1)&1;
			}

		}

        

		void softwareRender(){

			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

            int y0, y1;

            if( vertex[0][1] > vertex[1][1] ){
                y0 = vertex[1][1];
                y1 = vertex[0][1];
            }else{
                y1 = vertex[1][1];
                y0 = vertex[0][1];
            }

            for(int i = vertex[0][0]; i<vertex[1][0]; i++)
                for(int  j = y0; j<y1; j++)
                    putPixel(i,j);

		}

		bool onClick(int x, int y) 
		{
			// determinar la distancia del click a la línea
			// si es mejor a un umbral (e.g. 3 píxeles) entonces
			// retornas true
			return false;
		}

		void onMove(int x, int y)
		{
		}

};

#endif