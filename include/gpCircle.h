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
			// cout << "Se destruyo una linea" << endl;
		}

		void updateBoundingBox(){
			vertex[0][0] = bounding_box[0][0] = getCenter()[0]-r;
			vertex[0][1] = bounding_box[0][1] = getCenter()[1]-r;
			vertex[1][0] = bounding_box[1][0] = getCenter()[0]+r;
			vertex[1][1] = bounding_box[1][1] = getCenter()[1]+r;

			bounding_box[0][0] -= 3;
			bounding_box[0][1] -= 3;
			bounding_box[1][0] += 3;
			bounding_box[1][1] += 3;
		}

		bool onVertex(int x, int y){
			
			if( !gpShape::onVertex(x, y) )
				return false;

			int radius_x = getCenter()[0] - x;
			int radius_y = getCenter()[1] - y;

			radius_x = ABS(radius_x);
			radius_y = ABS(radius_y);			

			r = MIN(radius_x, radius_y);
			vertex[2][0] = (vertex[0][0]+vertex[1][0]) >> 1;
			vertex[2][1] = (vertex[0][1]+vertex[1][1]) >> 1;

			// updateBoundingBox();

			return true;
			
		}

		void setVertex(int n, int x, int y){

			gpShape::setVertex(n,x,y);

			int radius_x = abs(getCenter()[0] - vertex[0][0]);
			int radius_y = abs(getCenter()[1] - vertex[0][1]);

			r = radius_x < radius_y? radius_x:radius_y;

			// bounding_box[0][0] = getCenter()[0]-r;
			// bounding_box[0][1] = getCenter()[1]-r;
			// bounding_box[1][0] = getCenter()[0]+r;
			// bounding_box[1][1] = getCenter()[1]+r;

			// updateBoundingBox();

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

		bool onClick(int x, int y){

			if( !gpShape::onClick(x, y) )
				return false;

			int dx = getCenter(0) - x;
			int dy = getCenter(1) - y;

			return (dx*dx + dy*dy) <= (r*r);
		}

};

#endif