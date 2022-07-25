#ifndef GP_BEZIER_H
#define GP_BEZIER_H

#include <vector>
#include <map>
#include <utility>

#include "gpShape.h"

class gpBezier : public gpShape{
private:
    std::vector<int *> vertex;
    std::vector< std::vector<int *> > segments; 

public:
    gpBezier(int x0, int y0) : gpShape(x0, y0){
        vertex.push_back( new int[2]{x0, y0} );
    }

    ~gpBezier(){

        while( vertex.size() ){
            delete vertex.back();
            vertex.pop_back();
        }

    }

    void hardwareRender(){

    }
    
    void softwareRender(){

        glColor4f(border_color.x * border_color.w, border_color.y * border_color.w, border_color.z * border_color.w, border_color.w);

        if( vertex.size() <= 1 )
            return;

        int x0, x1, y0, y1;

        std::map< std::pair<int, int>, int > *ht;     
        for( float t = 0.0f; t<=1.001f; t+=0.015f ){

            ht = new std::map< std::pair<int, int>, int >();
            x0 = mix( 0, vertex.size()-2, 0, t, ht );            
            x1 = mix( 1, vertex.size()-1, 0, t, ht );            
            y0 = mix( 0, vertex.size()-2, 1, t, ht );            
            y1 = mix( 1, vertex.size()-1, 1, t, ht );            
            putPixel( lerp(x0, x1, t), lerp(y0, y1, t) );
            delete ht;

        }

    }

    void setVertex(int n, int x, int y){

        if( vertex.size()>=n ){
            vertex.push_back( new int[2]{x, y} );
            // printf("new shit\n");
        }else{
            vertex[n][0] = x;
            vertex[n][1] = y;
        }

    }

    bool onClick(int x, int y){
       return false; 
    }

    int lerp(int p0, int p1, float t){
        return (1.0f-t)*p0 + t*p1;
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

};



#endif