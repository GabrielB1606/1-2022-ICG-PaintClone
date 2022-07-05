#ifndef GP_CIRCLE_H
#define GP_CIRCLE_H

#include <math.h>
// #include "gpShape.hpp"

class gpCircle : public gpShape{

	private:

	public:
		gpCircle(int x0, int y0) : gpShape(x0, y0){
			
	
		}

		~gpCircle()
		{
			cout << "Se destruyo una linea" << endl;
		}

		void hardwareRender()
		{
			// despliegas la línea con el algoritmo de bresenham
			// setColor(color[0], color[1], color[2]);
			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

			float x = (float)center[0], y = (float)center[1];
			float radius = abs(x - (float)vertex[0][0]);
			float radiusy = abs(y - (float)vertex[0][1]);

			radius = radius > radiusy? radius:radiusy;
			// float radius = 10;
			int triangleAmount = ((int)radius)<<1;
			triangleAmount = triangleAmount < 100? triangleAmount:100;
			
			float twicePi = 6.28312f;

			glBegin(GL_TRIANGLE_FAN);

				glVertex2f(x, y); // center of circle
				for(int i = 0; i <= triangleAmount;i++) {
					glVertex2f(
						x + (radius * cos(i *  twicePi / triangleAmount)),
						y + (radius * sin(i * twicePi / triangleAmount))
						);
				}

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