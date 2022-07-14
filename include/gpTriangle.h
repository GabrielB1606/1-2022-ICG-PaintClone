#ifndef GP_TRIANGLE_H
#define GP_TRIANGLE_H

#include "gpShape.h"

class gpTriangle : public gpShape{

	private:
        int firstClick[2];

	public:
		gpTriangle(int x0, int y0) : gpShape(x0, y0){
			
	
		}

		~gpTriangle()
		{
			cout << "Se destruyo una linea" << endl;
		}

        void setVertex(int n, int x, int y){
            vertex[n][0] = x;
            vertex[n][1] = y;


            if( !vertice_mode ){
                if(n == 0){
                    firstClick[0] = x;
                    firstClick[1] = y ;
                
                    vertex[2][1] = firstClick[1];
                    vertex[0][0] = firstClick[0];
                }
				vertex[0][1] = vertex[1][1];
                // vertex[n][1] = MIN(vertex[0][1], vertex[1][1]);
                vertex[2][0] = (vertex[0][0]+vertex[1][0]) >> 1;
            }
            
        }

		void hardwareRender()
		{
			// despliegas la línea con el algoritmo de bresenham
			// setColor(color[0], color[1], color[2]);
			glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);

			// user putpixel de aquí en adelante... con Bresenham
			glBegin( GL_TRIANGLES );
				glVertex2i(vertex[0][0], window_height - vertex[0][1]);
				glVertex2i(vertex[1][0], window_height - vertex[1][1]);
				glVertex2i(vertex[2][0], window_height - vertex[2][1]);
			glEnd();
		}

		void softwareRender(){

			// glColor4f(color.x * color.w, color.y * color.w, color.z * color.w, color.w);
			color.x = 0;
			color.y = 0;
			color.z = 1;
            gpLine::draw( vertex[0][0], vertex[0][1], vertex[1][0], vertex[1][1], color );
			color.x = 0;
			color.y = 1;
			color.z = 0;
            gpLine::draw( vertex[1][0], vertex[1][1], vertex[2][0], vertex[2][1], color );
			color.x = 1;
			color.y = 0;
			color.z = 0;
            gpLine::draw( vertex[0][0], vertex[0][1], vertex[2][0], vertex[2][1], color );

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