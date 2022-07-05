#ifndef GP_ELLIPSE_H
#define GP_ELLIPSE_H

#include <math.h>
// #include "gpShape.hpp"

class gpEllipse : public gpShape{

	private:

	public:
		gpEllipse(int x0, int y0) : gpShape(x0, y0){
			
	
		}

		~gpEllipse()
		{
			cout << "Se destruyo una linea" << endl;
		}

		void hardwareRender()
		{
			// despliegas la línea con el algoritmo de bresenham
			// setColor(color[0], color[1], color[2]);
			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

			float x = (float)center[0], y = (float)center[1];
			float radiusx = abs(x - (float)vertex[0][0]);
			float radiusy = abs(y - (float)vertex[0][1]);

			// float radius = 10;
			int triangleAmount = ((int)max(radiusx, radiusy) )<<1;
			triangleAmount = triangleAmount < 100? triangleAmount:100;
			
			float theta = 6.28312f/triangleAmount;
			float sum_theta = 0;

			glBegin(GL_TRIANGLE_FAN);

				glVertex2f(x, y); // center of circle
				for(int i = 0; i <= triangleAmount;i++) {
					glVertex2f(
						x + (radiusx * cos( sum_theta) ),
						y + (radiusy * sin( sum_theta) )
					);
					sum_theta += theta;
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