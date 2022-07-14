#ifndef GP_CIRCLE_H
#define GP_CIRCLE_H

#include <math.h>
// #include "gpShape.hpp"

class gpCircle : public gpShape{

	private:
		int r;

	public:
		gpCircle(int x0, int y0) : gpShape(x0, y0){
			
	
		}

		~gpCircle()
		{
			cout << "Se destruyo una linea" << endl;
		}

		void setVertex(int n, int x, int y){

			gpShape::setVertex(n,x,y);

			int radius_x = abs(getCenter()[0] - vertex[0][0]);
			int radius_y = abs(getCenter()[1] - vertex[0][1]);

			r = radius_x < radius_y? radius_x:radius_y;

		}

		void hardwareRender()
		{
			// despliegas la línea con el algoritmo de bresenham
			// setColor(color[0], color[1], color[2]);
			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

			float x = (float)getCenter()[0], y = (float)getCenter()[1];

			// float radius = 10;
			int triangleAmount = ((int)r)<<1;
			triangleAmount = triangleAmount < 100? triangleAmount:100;
			
			float theta = 6.28312f/triangleAmount;
			float sum_theta = 0;

			glBegin(GL_TRIANGLE_FAN);

				glVertex2f(x, window_height - y); // center of circle
				for(int i = 0; i <= triangleAmount;i++) {
					glVertex2f(
						x + (r * cos(sum_theta) ),
						window_height - (y + (r * sin(sum_theta) ))
					);
					sum_theta += theta;
				}

			glEnd();

		}

		void softwareRender(){

			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

			int point[2] = {0, r};

			int d = 1-r;

			while( point[1]>point[0] ){
				if( d < 0 )
					d = d + 2*point[0] + 3;
				else{
					d = d + 2 * (point[0] - point[1]) + 5;
					point[1]--;
				}
				point[0]++;

				for(int i = -1; i<2; i+=2)
					for(int j = -1; j<2; j+=2){
						putPixel(getCenter()[0]+j*point[0], getCenter()[1]+i*point[1] );
						putPixel(getCenter()[0]+j*point[1], getCenter()[1]+i*point[0] );
					}
				
			}

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