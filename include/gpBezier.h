#ifndef GP_BEZIER_H
#define GP_BEZIER_H

#include <vector>
#include <map>
#include <list>
#include <utility>

#include "gpShape.h"

class gpBezier : public gpShape{
    private:
        std::vector<int *> vertex;
        std::deque<int *> segments; 
        float t = 0.1f;

    public:

        std::string toString(){
			std::stringstream sstr;
			
			sstr<<"BEZIER" << vertex.size() << " ";

			for(size_t i = 0; i<vertex.size(); i++)
				sstr << vertex[i][0] << " " << vertex[i][1] << " ";

			sstr << border_color.x << " " << border_color.y << " " << border_color.z;

			return sstr.str();

		}	

        gpBezier(int x0, int y0) : gpShape(x0, y0){
            shape = DrawBezier;
            // t = 1.0f/(float)n_segments;
            vertex.reserve(21);
            vertex.push_back( new int[2]{x0, y0} );
        }

        ~gpBezier(){

            while( vertex.size() ){
                delete vertex.back();
                vertex.pop_back();
            }

        }

        void hardwareRender(){
            glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);
            glBegin( GL_LINE_STRIP );
            for(size_t i = 0; i<segments.size(); i++)
                glVertex2i( segments[i][0], segments[i][1] );
            glEnd();
            glFlush();
        }
        
        void setT(int n_segments){
            this->t = 1.0f/(float)n_segments;
            updateSegments();
        }

        void updateSegments(){

            if( vertex.size() <=2 )
                return;

            while( segments.size() ){
                delete segments.back();
                segments.pop_back();
            }

            int x0, x1, y0, y1;

            std::map< std::pair<int, int>, int > *ht;   

            for( float i = 0.0f; i<=1.001f; i+=t ){

                ht = new std::map< std::pair<int, int>, int >();
                x0 = mix( 0, vertex.size()-2, 0, i, ht );            
                x1 = mix( 1, vertex.size()-1, 0, i, ht );            
                y0 = mix( 0, vertex.size()-2, 1, i, ht );            
                y1 = mix( 1, vertex.size()-1, 1, i, ht );            
                
                segments.push_back( new int[2]{lerp(x0, x1, i), lerp(y0, y1, i)} );

                delete ht;

            }

            // updateBoundingBox();
        }

        void softwareRender(){

            if(vertex.size() <= 1)
                return;

            if( vertex.size() <= 2 ){
                gpLine::draw( vertex[0][0], vertex[0][1], vertex[1][0], vertex[1][1], border_color);
                return;
            }
            
            for(size_t i = 1; i<segments.size(); i++)
                gpLine::draw( segments[i-1][0], segments[i-1][1], segments[i][0], segments[i][1], border_color );

        }

        void setVertex(int n, int x, int y){

            if(n >= 20)
                return;

            if( vertex.size() > (size_t)n ){
                vertex[n][0] = x;
                vertex[n][1] = y;
            }else{
                // printf("push vector size = %d, n = %d\n", vertex.size(), n);
                vertex.push_back( new int[2]{x, y} );
            }

            updateSegments();

        }

        bool onClick(int x, int y){
            if( !gpShape::onClick(x, y) )
                return false;

            // printf("in bounding\n");

            int dx, dy;

            for(size_t i = 0; i<segments.size(); i++){
                dx = segments[i][0] - x;
                dy = segments[i][1] - y;
                if( (dx*dx + dy*dy) <= (25) )
                    return true;
            }

            return false; 
        }

        int lerp(int p0, int p1, float t){
            return (int)((1.0f-t)*p0 + t*p1);
        }

        int mix( int i, int j, int axis, float t, std::map<std::pair<int, int>, int>* ht ){

            if( ht->find( std::pair<int, int>(vertex[i][axis], vertex[j][axis]) ) != ht->end() )
                return ht->at( std::pair<int, int>(vertex[i][axis], vertex[j][axis]) );

            int ans;
            if( j-i == 1 ){
                ans = lerp( vertex[i][axis], vertex[j][axis], t );
            }else{
                ans = lerp( mix(i, j-1, axis, t, ht), mix(i+1, j, axis, t, ht), t);
            }

            (*ht)[ std::pair<int, int>(vertex[i][axis], vertex[j][axis]) ] = ans;
            return ans;

        }

        bool onVertex(int x, int y){
			int dx, dy;
			// int diff[2] = {x - mouse_pos[0], y - mouse_pos[1]};

            for(size_t i = 0; i<vertex.size(); i++){
                dx = vertex[i][0] - x;
                if( (ABS(dx)) <= 8 ){
                    dy = vertex[i][1] - y;
                    if( (ABS(dy)) <= 8){
                        setVertex(i, x, y);
                        return true;
                    }
                }
            }


			return false;
		}

        void updateBoundingBox(){

            bounding_box[1][0] = bounding_box[0][0] = vertex[0][0];
            bounding_box[1][1] = bounding_box[0][1] = vertex[0][1];

            for(size_t i = 1; i<segments.size(); i+=2){
                bounding_box[0][0] = MIN(bounding_box[0][0], segments[i][0]);
                bounding_box[0][1] = MIN(bounding_box[0][1], segments[i][1]);
                bounding_box[1][0] = MAX(bounding_box[1][0], segments[i][0]);
                bounding_box[1][1] = MAX(bounding_box[1][1], segments[i][1]);
            }

        }

        void onMove(int x, int y){
            if( gpShape::onClick(x, y) && selected ){
				
				int diff[2] = {x - mouse_pos[0], y - mouse_pos[1]};

				for(size_t i =0; i<vertex.size(); i++){
					vertex[i][0] += diff[0];
					vertex[i][1] += diff[1];
				}

                for(size_t i =0; i<segments.size(); i++){
					segments[i][0] += diff[0];
					segments[i][1] += diff[1];
				}

				for(int i = 0; i< 2; i++){
					bounding_box[i][0] += diff[0];
					bounding_box[i][1] += diff[1];
				}
			}
        }

        void renderBoundingBox(){
            
            glColor3f(70, 70, 70);
            glLineStipple(1, 0xAAAA);  
            glEnable(GL_LINE_STIPPLE);
            glBegin(GL_LINE_STRIP);
                for(size_t i = 0; i<vertex.size(); i++)
                    glVertex2i(vertex[i][0], 720 - vertex[i][1]);    
            glEnd();
            glDisable(GL_LINE_STIPPLE);

            for(size_t i = 0; i<vertex.size(); i++)
                gpShape::draw7x7( vertex[i][0], vertex[i][1] );

        }

};



#endif