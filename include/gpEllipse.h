#ifndef GP_ELLIPSE_H
#define GP_ELLIPSE_H

#include <math.h>
// #include "gpShape.hpp"

class gpEllipse : public gpShape{

	private:
		int rx, ry, rxrx, ryry;

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
			rxrx = rx*rx;
			ryry = ry*ry; 

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

		static void draw(int center[2], int rx, int ry){
			int x, y, d;
			int rxrx = rx*rx;
			int ryry = ry*ry;

			x = 0;
			y = ry;

			d = ry * ((ry - rxrx)<<2) + rxrx;

			while( ((ryry * (x + 1)) << 1) < rxrx * ((y<<1) - 1) ){
				putPixel(center[0] + x, center[1] + y);	
				putPixel(center[0] - x, center[1] + y);	
				putPixel(center[0] + x, center[1] - y);	
				putPixel(center[0] - x, center[1] - y);	
				if(d < 0)
					d += ((ryry * ((x<<1) + 3))<<2);
				else {
					d += ((ryry * ((x<<1) + 3) + rxrx* (-(y<<1) + 2))<<2);
					y --;
				}
				x ++;
			}

			d = ryry*( ((x*x+x)<<2) +1) + rxrx*( ((y*y-(y<<1))<<2) +4 )- ((rxrx*ryry) << 2);

			while( y>=0 ){
				putPixel(center[0] + x, center[1] + y);	
				putPixel(center[0] - x, center[1] + y);	
				putPixel(center[0] + x, center[1] - y);	
				putPixel(center[0] - x, center[1] - y);	
				if( d<0 ){
					d += (ryry*((x + 1)<<1) + rxrx *(-(y<<1) + 3)) << 2;
					x++;
				}else{
					d += (rxrx<<2)*(-(y<<1) + 3);
				}
				y--;
			}
		}

		void softwareRenderFill(){

			if( rx < ry ){
				for(int i = 0; i<rx; i++)
					gpCircle::draw(getCenter(), i);
				
				for(int i = rx; i<ry; i++)
					gpEllipse::draw(getCenter(), rx, i);
			}else{
				for(int i = 0; i<ry; i++)
					gpCircle::draw(getCenter(), i);
				
				for(int i = ry; i<rx; i++)
					gpEllipse::draw(getCenter(), i, ry);
			}

		}

		void softwareRenderBorder(){

			int x, y, d;

			x = 0;
			y = ry;

			d = ry * ((ry - rxrx)<<2) + rxrx;

			while( ((ryry * (x + 1)) << 1) < rxrx * ((y<<1) - 1) ){
				if(d < 0)
					d += ((ryry * ((x<<1) + 3))<<2);
				else {
					d += ((ryry * ((x<<1) + 3) + rxrx* (-(y<<1) + 2))<<2);
					y --;
				}
				x ++;
				putPixel(getCenter()[0] + x, getCenter()[1] + y);	
				putPixel(getCenter()[0] - x, getCenter()[1] + y);	
				putPixel(getCenter()[0] + x, getCenter()[1] - y);	
				putPixel(getCenter()[0] - x, getCenter()[1] - y);	
			}

			d = ryry*( ((x*x+x)<<2) +1) + rxrx*( ((y*y-(y<<1))<<2) +4 )- ((rxrx*ryry) << 2);

			while( y>0 ){
				if( d<0 ){
					d += (ryry*((x + 1)<<1) + rxrx *(-(y<<1) + 3)) << 2;
					x++;
				}else{
					d += (rxrx<<2)*(-(y<<1) + 3);
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