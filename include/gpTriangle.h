#ifndef GP_TRIANGLE_H
#define GP_TRIANGLE_H

#include "gpShape.h"

class gpTriangle : public gpShape{

	private:
        int firstClick[2];
		int main_axis[3] = {0, 0, 0};
		float m[3];

	public:
		gpTriangle(int x0, int y0) : gpShape(x0, y0){
			

	
		}

		~gpTriangle()
		{
			cout << "Se destruyo una linea" << endl;
		}

		void updateSlope(int n){
			
			int dest = MOD_INC(n, 3);

			int sec_axis = (main_axis[n]+1)&1;
			
			float denominador = (float)(vertex[dest][main_axis[n]] - vertex[n][main_axis[n]]);

			if( denominador != 0 )
				m[n] = (float)(vertex[dest][sec_axis] - vertex[n][sec_axis])/denominador;
			else{
				m[n] = 1;
			}

			if( m[n]<-1 || m[n]>1 ){
				int sw = main_axis[n];
				main_axis[n] = sec_axis;
				sec_axis = sw;
				denominador = (float)(vertex[dest][main_axis[n]] - vertex[n][main_axis[n]]);

				if( denominador != 0 )
					m[n] = (float)(vertex[dest][sec_axis] - vertex[n][sec_axis])/denominador;
				else
					m[n] = 1;
			}			
		
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

				updateSlope(0);
				updateSlope(1);
				updateSlope(2);
            }else{
				updateSlope(n);
				updateSlope( MOD_DEC(n, 3) );
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

		void renderBoundingBox(){

			glColor3f(70, 70, 70);
			for(int i = 0; i<3; i++)
				for(int x = -3; x<=3; x++)
					for(int y = -3; y<=3; y++)
						putPixel( vertex[i][0]+x, vertex[i][1]+y );

		}

		void softwareRender(){

			float point[2][2] = {
				{(float)vertex[1][0], (float)vertex[1][1]},
				{(float)vertex[1][0], (float)vertex[1][1]}
			};

			float final[2] = {
				(float)vertex[0][main_axis[0]],
				(float)vertex[2][main_axis[1]]
			};

			int sec_axis[2] = {
				(main_axis[0]+1)&1,
				(main_axis[1]+1)&1
			};

			int step[2] = {
				vertex[0][main_axis[0]] > vertex[1][main_axis[0]]? 1:-1,
				vertex[2][main_axis[1]] > vertex[1][main_axis[1]]? 1:-1
			};

			unsigned char drawing = 0b11;

			while( drawing ){
				
				gpLine::draw( (int)point[0][0], (int)point[0][1], (int)point[1][0], (int)point[1][1], fill_color );
				
				glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);

				if( (int)point[0][main_axis[0]] != (int)final[0] ){
					putPixel( ROUNDNUM( point[0][0] ),  ROUNDNUM( point[0][1] ) );
					point[0][sec_axis[0]] += step[0]*m[0];
					point[0][main_axis[0]] += step[0];
				}else{
					drawing &= 0b10;
				}

				if( (int)point[1][main_axis[1]] != (int)final[1] ){
					putPixel( ROUNDNUM( point[1][0] ),  ROUNDNUM( point[1][1] ) );
					point[1][sec_axis[1]] += step[1]*m[1];
					point[1][main_axis[1]] += step[1];
				}else{
					drawing &= 0b01;
				}

			}
			gpLine::draw( (int)point[0][0], (int)point[0][1], (int)point[1][0], (int)point[1][1], border_color );
		}

		int areaTimes2(int x1, int y1,int x2, int y2,int x3, int y3){
			int area = x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2);
			return ABS(area);
		}

		bool onClick(int x, int y) 
		{
			
			int area_ABC = areaTimes2( vertex[0][0], vertex[0][1], vertex[1][0], vertex[1][1], vertex[2][0], vertex[2][1] );
			
			int area_PBC = areaTimes2( x, y, vertex[1][0], vertex[1][1], vertex[2][0], vertex[2][1] );
			
			int area_PAC = areaTimes2( vertex[0][0], vertex[0][1], x, y, vertex[2][0], vertex[2][1] );
			
			int area_PAB = areaTimes2( vertex[0][0], vertex[0][1], vertex[1][0], vertex[1][1], x, y );

			// printf("%f = %f + %f + %f\n",area_ABC, area_PBC, area_PAC, area_PAB);

			area_ABC -= ( ABS(area_PBC) + ABS(area_PAC) + ABS(area_PAB) );
			
			// printf(" = %f \n",area_ABC);

			return area_ABC == 0;
		}

		// void onMove(int x, int y){
		// }

		// void select(bool s){

		// }

};

#endif