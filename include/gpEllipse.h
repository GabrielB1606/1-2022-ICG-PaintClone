#ifndef GP_ELLIPSE_H
#define GP_ELLIPSE_H

#include <math.h>
// #include "gpShape.hpp"

class gpEllipse : public gpShape{

	private:
		int rx, ry;

	public:
		gpEllipse(int x0, int y0) : gpShape(x0, y0){
			
	
		}

		~gpEllipse()
		{
			cout << "Se destruyo una linea" << endl;
		}

		void setVertex(int n, int x, int y){

			gpShape::setVertex(n,x,y);

			rx = abs(getCenter()[0] - vertex[0][0]);
			ry = abs(getCenter()[1] - vertex[0][1]);

		}

		void hardwareRenderFill()
		{
			int x = getCenter()[0], y = getCenter()[1];

			int triangleAmount = ( MAX(rx, ry) )<<1;
			triangleAmount = triangleAmount < 100? triangleAmount:100;
			
			float theta = 6.28312f/triangleAmount;
			float sum_theta = 0;

			glBegin(GL_TRIANGLE_FAN);

				glVertex2i(x, window_height - y); // center of circle
				for(int i = 0; i <= triangleAmount;i++) {
					glVertex2f(
						x + (rx * cos(sum_theta) ),
						window_height - (y + (ry * sin(sum_theta) ))
					);
					sum_theta += theta;
				}

			glEnd();

		}

		void hardwareRenderBorder(){
			int x = getCenter()[0], y = getCenter()[1];

			int triangleAmount = ( MAX(rx, ry) )<<1;
			triangleAmount = triangleAmount < 100? triangleAmount:100;
			
			float theta = 6.28312f/triangleAmount;
			float sum_theta = 0;

			glBegin(GL_LINE_LOOP);

				// glVertex2i(x, window_height - y); // center of circle
				for(int i = 0; i <= triangleAmount;i++) {
					glVertex2f(
						x + (rx * cos(sum_theta) ),
						window_height - (y + (ry * sin(sum_theta) ))
					);
					sum_theta += theta;
				}

			glEnd();
		}

		void softwareRenderFill(){}

		void softwareRenderBorder(){

			int x, y, d;

			x = 0;
			y = ry;

			d = ry * (4 * ry - 4 * rx*rx) + rx*rx;

			while(ry*ry * 2 * (x + 1) < rx*rx * (2*y - 1)){
				if(d < 0)
					d = d + 4 * (ry*ry * (2*x + 3));
				else {
					d = d + 4 * (ry*ry * (2*x + 3) + rx*rx* (-2*y + 2));
					y = y - 1;
				}
				x = x + 1;
				putPixel(getCenter()[0] + x, getCenter()[1] + y);	
				putPixel(getCenter()[0] - x, getCenter()[1] + y);	
				putPixel(getCenter()[0] + x, getCenter()[1] - y);	
				putPixel(getCenter()[0] - x, getCenter()[1] - y);	
			}

			d = ry*ry*(4*x*x+4*x+1) + rx*rx*(4*y*y-8*y+4)- 4*rx*rx *ry*ry;

			while( y>0 ){
				if( d<0 ){
					d += 4 * (ry*ry*(2*x + 2) + rx*rx *(-2*y + 3));
					x++;
				}else{
					d += 4*rx*rx* (-2*y + 3);
				}
				y--;
				putPixel(getCenter()[0] + x, getCenter()[1] + y);	
				putPixel(getCenter()[0] - x, getCenter()[1] + y);	
				putPixel(getCenter()[0] + x, getCenter()[1] - y);	
				putPixel(getCenter()[0] - x, getCenter()[1] - y);	
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