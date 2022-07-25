#ifndef GP_LINE_H
#define GP_LINE_H

// #include "gpShape.hpp"

class gpLine : public gpShape{

	private:

		float m;
		int main_axis = 0;

	public:
		gpLine(int x0, int y0) : gpShape(x0, y0){
		}

		gpLine(int x0, int y0, int x1, int y1, ImVec4 color) : gpShape(x0, y0, x1, y1, color){
		}

		gpLine(ImVec4 color): gpShape(color){
			vertex[2][0] = vertex[1][0] = vertex[0][0] = 1;
			vertex[2][1] = vertex[1][1] = vertex[0][1] = 1;
		}

		~gpLine()
		{
			// cout << "Se destruyo una linea" << endl;
		}

		bool updateSlope(){
	
			if( vertex[0][main_axis] > vertex[1][main_axis] ){
				// float tmpx = vertex[0][0], tmpy = vertex[0][1];	
				// vertex[0][0] = vertex[1][0];
				// vertex[0][1] = vertex[1][1];
				// vertex[1][0] = tmpx;
				// vertex[1][1] = tmpy;
				swap( vertex[0], vertex[1] );
				vertex_dragging = (vertex_dragging + 1)&1;
			}

			int sec_axis = (main_axis+1)&1;
			m = (float)(vertex[1][sec_axis] - vertex[0][sec_axis])/(vertex[1][main_axis] - vertex[0][main_axis]);

			return (m>=-1 && m<=1);
		}

		void setVertex(int n, int x, int y){

			gpShape::setVertex(n,x,y);

			if( !updateSlope() ){
				main_axis = (main_axis+1)&1;
				updateSlope();
			}

		}

		static void drawHorizontal(int x0, int x1, int y, ImVec4 color){

			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

			if(x0 > x1)
				swap(x0, x1);

			for(;x0<=x1; x0++)
				putPixel(x0, y);

		}

		static void draw(int x0, int y0, int x1, int y1, ImVec4 color){
			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);
			glBegin(GL_LINES);
				glVertex2i(x0, 720 - y0);
				glVertex2i(x1, 720 - y1);
			glEnd();

			// float point[2];
			// float final;

			// int main_axis;
			// int sec_axis;
			// float vertex[2][2] = { {x0, y0}, {x1, y1} };

			// if( ABS(x1-x0) > ABS(y1-y0) ){
			// 	main_axis = 0;
			// 	sec_axis = 1;
			// }else{
			// 	main_axis = 1;
			// 	sec_axis = 0;
			// }

			// if( vertex[0][main_axis] > vertex[1][main_axis] ){
			// 	float tmpx = vertex[0][0], tmpy = vertex[0][1];	
			// 	vertex[0][0] = vertex[1][0];
			// 	vertex[0][1] = vertex[1][1];
			// 	vertex[1][0] = tmpx;
			// 	vertex[1][1] = tmpy;
			// }
			
			// float m = (float)(vertex[1][sec_axis] - vertex[0][sec_axis])/(vertex[1][main_axis] - vertex[0][main_axis]);

			// point[0] = (float)vertex[0][0];
			// point[1] = (float)vertex[0][1];
			// final = (float)vertex[1][main_axis];

			// glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

			// for(; point[main_axis]<final; point[main_axis]++){
			// 	putPixel(ROUNDNUM(point[0]), ROUNDNUM(point[1]));
			// 	point[sec_axis] += m;
			// }


			// gpLine l(x0, y0);
			// l.setVertex(1, x1, y1);
			// l.setBorderColor(color);
			// l.softwareRender();

		}

		void hardwareRender()
		{
			// despliegas la línea con el algoritmo de bresenham
			// setColor(color[0], color[1], color[2]);
			glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);

			// user putpixel de aquí en adelante... con Bresenham
			glBegin(GL_LINES);
				glVertex2i(vertex[0][0], window_height - vertex[0][1]);
				glVertex2i(vertex[1][0], window_height - vertex[1][1]);
			glEnd();
		}

		void softwareRender(){
			
			glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);

			float point[2];
			float final;

			point[0] = (float)vertex[0][0];
			point[1] = (float)vertex[0][1];
			final = (float)vertex[1][main_axis];


			int sec_axis = (main_axis+1)&1;

			for(; point[main_axis]<final; point[main_axis]++){
				putPixel(ROUNDNUM(point[0]), ROUNDNUM(point[1]));
				point[sec_axis] += m;
			}

		}

		bool onClick(int x, int y) 
		{
			int point[] = {x, y};

			if(point[main_axis]+5 < vertex[0][main_axis] || point[main_axis]-5 > vertex[1][main_axis]  )
				return false;

			int sec_axis = (main_axis+1)&1;

			int b = vertex[1][sec_axis] - m*vertex[1][main_axis];
			int should_be = m*point[main_axis] + b;

			int d = should_be - point[sec_axis];

			// int d = (int)ABS( ((int)m*point[main_axis] + b - point[sec_axis]) ) * Q_rsqrt( (m*m) + 1 );

			// double m = ( vertex[1][1] - vertex[0][1])/ ( vertex[1][0] - vertex[0][0] );
			// double b = vertex[1][1] - m*vertex[1][0];
			// double d = ABS( (m*x-y+b) )/sqrt((m*m)+1);

			cout << ABS(d) << "\n";
			
			return ABS(d) <= 5;
			
		}

		void renderBoundingBox(){
			glColor3f(70, 70, 70);
			for(int x = -3; x<=3; x++)
				for(int y = -3; y<=3; y++){
					putPixel( vertex[0][0]+x, vertex[0][1]+y );
					putPixel( vertex[1][0]+x, vertex[1][1]+y );
				}
		}

		void onMove(int x, int y)
		{
		}

		void select(bool s){
			selected = s;
		}

};

#endif