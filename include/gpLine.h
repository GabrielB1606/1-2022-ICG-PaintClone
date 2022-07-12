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

		~gpLine()
		{
			cout << "Se destruyo una linea" << endl;
		}

		bool updateSlope(){
	
			if( vertex[0][main_axis] > vertex[1][main_axis] ){
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

		void hardwareRender()
		{
			// despliegas la línea con el algoritmo de bresenham
			// setColor(color[0], color[1], color[2]);
			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

			// user putpixel de aquí en adelante... con Bresenham
			glBegin(GL_LINES);
				glVertex2i(vertex[0][0], vertex[0][1]);
				glVertex2i(vertex[1][0], vertex[1][1]);
			glEnd();
		}

		void softwareRender(){
			
			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

			float point[2] = { (float)vertex[0][0], (float)vertex[0][1] };

			int sec_axis = (main_axis+1)&1;

			for(; point[main_axis]<vertex[1][main_axis]; point[main_axis]++){
				putPixel(ROUNDNUM(point[0]), ROUNDNUM(point[1]));
				point[sec_axis] += m;
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