#ifndef GP_ELLIPSE_H
#define GP_ELLIPSE_H

#include <math.h>
// #include "gpShape.hpp"

class gpEllipse : public gpShape{

	private:
		int rx, ry, rxrx, ryry;

	public:

		std::string toString(){
			std::stringstream sstr;

			if(filled)
				sstr << "FILLED_";
			
			sstr<<"ELLIPSE ";

			for(int i = 0; i<4; i++)
				sstr << vertex[0][i] << " ";

			sstr << border_color.x << " " << border_color.y << " " << border_color.z;
			
			if(filled)		
				sstr << " " << fill_color.x << " " << fill_color.y << " " << fill_color.z;	

			return sstr.str();
		}	

		gpEllipse(int x0, int y0) : gpShape(x0, y0){
			shape = DrawEllipse;
	
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

				glVertex2i(x, y); // center of circle
				for(int i = 0; i <= triangleAmount;i++) {
					glVertex2f(
						x + (rx * cos(sum_theta) ),
						(y + (ry * sin(sum_theta) ))
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
						(y + (ry * sin(sum_theta) ))
					);
					sum_theta += theta;
				}

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
		
		void ellipsePoints(int x, int y, int center[2]){
			putPixel(center[0] + x, center[1] + y);	
			putPixel(center[0] - x, center[1] + y);	
			putPixel(center[0] + x, center[1] - y);	
			putPixel(center[0] - x, center[1] - y);	
		}

		void softwareRender(){

			int x, y, d;

			x = 0;
			y = ry;

			d = ry * ((ry - rxrx)<<2) + rxrx;

			while( ((ryry * (x + 1)) << 1) < rxrx * ((y<<1) - 1) ){

				glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);
				ellipsePoints(x, y, getCenter());

				if(filled){
					glColor4f(fill_color.x * fill_color.w, fill_color.y * fill_color.w, fill_color.z * fill_color.w, fill_color.w);
					for(int i = 0; i<y; i++)
						ellipsePoints(x, i, getCenter());
				}

				if(d < 0)
					d += ((ryry * ((x<<1) + 3))<<2);
				else {
					d += ((ryry * ((x<<1) + 3) + rxrx* (-(y<<1) + 2))<<2);
					y --;
				}
				x ++;

			}

			d = ryry*( ((x*x+x)<<2) +1) + rxrx*( ((y*y-(y<<1))<<2) +4 )- ((rxrx*ryry) << 2);

			while( y>0 ){

				glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);
				ellipsePoints(x, y, getCenter());

				if(filled){
					glColor4f(fill_color.x * fill_color.w, fill_color.y * fill_color.w, fill_color.z * fill_color.w, fill_color.w);
					for(int i = 0; i<y; i++)
						ellipsePoints(x, i, getCenter());
				}

				if( d<0 ){
					d += (ryry*((x + 1)<<1) + rxrx *(-(y<<1) + 3)) << 2;
					x++;
				}else{
					d += (rxrx<<2)*(-(y<<1) + 3);
				}
				y--;
			}
		}

		bool onVertex(int x, int y, bool move = true){
			
			if( !gpShape::onVertex(x, y, move) )
				return false;

			if(move){
				rx = getCenter()[0] - x;
				ry = getCenter()[1] - y;

				rx = ABS(rx);
				ry = ABS(ry);

				rxrx = rx*rx;
				ryry = ry*ry;			

				vertex[2][0] = (vertex[0][0]+vertex[1][0]) >> 1;
				vertex[2][1] = (vertex[0][1]+vertex[1][1]) >> 1;
			}

			// updateBoundingBox();

			return true;
			
		}		

		bool onClick(int x, int y){

			if( !gpShape::onClick(x, y) )
				return false;
			
			int x_h = x-getCenter()[0];
			int y_k = y-getCenter()[1];

			int d = (x_h*x_h/rxrx) + (y_k *y_k/ryry);

			return !d;
		}


};

#endif