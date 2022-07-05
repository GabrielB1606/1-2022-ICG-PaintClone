#ifndef GP_TRIANGLE_H
#define GP_TRIANGLE_H

#include "gpShape.h"

class gpTriangle : public gpShape{

	private:

	public:
		gpTriangle(int x0, int y0) : gpShape(x0, y0){
			
	
		}

		~gpTriangle()
		{
			cout << "Se destruyo una linea" << endl;
		}

		void hardwareRender()
		{
			// despliegas la línea con el algoritmo de bresenham
			// setColor(color[0], color[1], color[2]);
			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

			// user putpixel de aquí en adelante... con Bresenham
			glBegin( GL_TRIANGLES );
				glVertex2i(vertex[0][0], vertex[1][1]);
				glVertex2i(vertex[1][0], vertex[1][1]);
				glVertex2i( (vertex[0][0]+vertex[1][0])>>1 , vertex[0][1]);
			glEnd();
		}

		void softwareRender(){

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