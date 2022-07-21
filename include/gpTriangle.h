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

		void hardwareRenderFill()
		{
			glBegin( GL_TRIANGLES );
				glVertex2i(vertex[0][0], window_height - vertex[0][1]);
				glVertex2i(vertex[1][0], window_height - vertex[1][1]);
				glVertex2i(vertex[2][0], window_height - vertex[2][1]);
			glEnd();
		}

		void hardwareRenderBorder(){
			glBegin( GL_LINE_LOOP );
				glVertex2i(vertex[0][0], window_height - vertex[0][1]);
				glVertex2i(vertex[1][0], window_height - vertex[1][1]);
				glVertex2i(vertex[2][0], window_height - vertex[2][1]);
			glEnd();
		}

		void hardwareRender(){
			glColor4f(fill_color.x * fill_color.w, fill_color.y * fill_color.w, fill_color.z * fill_color.w, fill_color.w);
			hardwareRenderFill();
			
			glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);
			hardwareRenderBorder();
		}

		void softwareRender(){

            gpLine::draw( vertex[0][0], vertex[0][1], vertex[1][0], vertex[1][1], border_color );
            gpLine::draw( vertex[1][0], vertex[1][1], vertex[2][0], vertex[2][1], border_color );
            gpLine::draw( vertex[0][0], vertex[0][1], vertex[2][0], vertex[2][1], border_color );

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