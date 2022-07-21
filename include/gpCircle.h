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

		void hardwareRenderFill(){

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

		void hardwareRenderBorder(){
			float x = (float)getCenter()[0], y = (float)getCenter()[1];

			// float radius = 10;
			int triangleAmount = ((int)r)<<1;
			triangleAmount = triangleAmount < 100? triangleAmount:100;
			
			float theta = 6.28312f/triangleAmount;
			float sum_theta = 0;

			glBegin(GL_LINE_LOOP);

				// glVertex2f(x, window_height - y); // center of circle
				for(int i = 0; i <= triangleAmount;i++) {
					glVertex2f(
						x + (r * cos(sum_theta) ),
						window_height - (y + (r * sin(sum_theta) ))
					);
					sum_theta += theta;
				}

			glEnd();
		}

		void hardwareRender(){
			glColor4f(fill_color.x * fill_color.w, fill_color.y * fill_color.w, fill_color.z * fill_color.w, fill_color.w);
			hardwareRenderFill();
			
			glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);
			hardwareRenderBorder();
		}

		static void circlePoints(int x, int y, int center[2]){
			int point[] = {x, y};
			for(int i = -1; i<2; i+=2)
				for(int j = -1; j<2; j+=2){
					putPixel(center[0]+j*point[0], center[1]+i*point[1] );
					putPixel(center[0]+j*point[1], center[1]+i*point[0] );
				}
		}

		void softwareRender(){

			int point[2] = {0, r};

			int d = 1-r;

			while( point[1]>point[0] ){
				
				glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);
				circlePoints(point[0], point[1], getCenter());

				glColor4f(fill_color.x * fill_color.w, fill_color.y * fill_color.w, fill_color.z * fill_color.w, fill_color.w);
				for(int i = 0; i<point[1]; i++)
					circlePoints(point[0], i, getCenter());
				
				
				if( d < 0 )
					d = d + (point[0]<<1) + 3;
				else{
					d = d + ((point[0] - point[1])<<1) + 5;
					point[1]--;
				}
				point[0]++;
				
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