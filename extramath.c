
#include "extramath.h"

Vector2D init_vec2(double x1,double y1){
    Vector2D new;
    new.x=x1;
    new.y=y1;
    return new;
}
Vector2D multiply_vec2(Vector2D v1, double lambda){
    Vector2D new;
    new.x=v1.x*lambda;
    new.y=v1.y*lambda;
    return new;
}
Vector2D add_vec2(Vector2D v1, Vector2D v2){
    Vector2D new;
    new.x=v1.x+v2.x;
    new.y=v1.y+v2.y;
    return new;
}
double distance_vec2(Vector2D v1, Vector2D v2){
    double length=sqrt((v2.x-v1.x)*(v2.x-v1.x)+(v2.y-v1.y)*(v2.y-v1.y));
    return length;
}
bool overlapping(Vector2D pos1, double radius1, Vector2D pos2,double radius2){
    if(distance_vec2(pos1,pos2)<=radius1+radius2){
        return true;
    }
    return false;
}
double clampd(double num, double min, double max) {
    if (num > max) {
        return max;
    } else if (num < min) {
        return min;
    }
    return num;
}
