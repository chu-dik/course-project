/*#include <iostream>
#include "render.h"

Point::Point(float xn, float yn, float zn){
    x = xn;
    y = yn;
    z = zn;
}

Point::Point(const Point &p){
    x = p.x;
    y = p.y;
    z = p.z;
}

Point::Point(){}

Polygon::Polygon(){}
Polygon::Polygon(Point p1n, Point p2n, Point p3n){
    p1 = p1n;
    p2 = p2n;
    p3 = p3n;
}

Polygon::Polygon(const Polygon &p){
    p1 = p.p1;
    p2 = p.p2;
    p3 = p.p3;
}

PolygonList::PolygonList(){}
PolygonList::PolygonList(Polygon plgnn, PolygonList *nextn){
    plgn = plgnn;
    next = nextn;
}

Object::Object(){}
Object::Object(PolygonList *plgnlist,QRgb clr)
{
    head = plgnlist;
    color = clr;
}

ObjectList::ObjectList(){}
ObjectList::ObjectList(Object objn, ObjectList* nextn){
    obj = objn;
    next = nextn;
}

Camera::Camera(){}
Camera::Camera(Point pn, float ax, float ay, float az){
    pos = pn;
    focus = pos.z + 100;

    anglex = ax;
    angley = ay;
    anglez = az;
}

Light::Light(){}
Light::Light(Point pn, QRgb clr){
    pos = pn;
    color = clr;
}

Point rotate_point_x(Point p,float sin_al,float cos_al){
    float y = p.y * cos_al - p.z * sin_al;
    float z = p.y * sin_al + p.z * cos_al;

    return Point(p.x, y, z);
}

Point rotate_point_y(Point p,float sin_al,float cos_al){
    float x = p.x * cos_al - p.z * sin_al;
    float z = p.x * sin_al + p.z * cos_al;
    return Point(x, p.y, z);
}

Point rotate_point_z(Point p,float sin_al,float cos_al){
    float x = p.x * cos_al - p.y * sin_al;
    float y = p.x * sin_al + p.y * cos_al;

    return Point(x, y, p.z);
}

Polygon rotate_polygon(Polygon p, float ax,float ay,float az)
{
    float rad = 57.296;
    float sina = sin(ax/rad);
    float cosa = sin(ax/rad);
    p.p1 = rotate_point_x(p.p1,sina, cosa);
    p.p2 = rotate_point_x(p.p2,sina, cosa);
    p.p3 = rotate_point_x(p.p3,sina, cosa);

    sina = sin(ay/rad);
    cosa = cos(ay/rad);
    p.p1 = rotate_point_y(p.p1,sina, cosa);
    p.p2 = rotate_point_y(p.p2,sina, cosa);
    p.p3 = rotate_point_y(p.p3,sina, cosa);

    sina = sin(ax/rad);
    cosa = cos(ax/rad);
    p.p1 = rotate_point_z(p.p1,sina, cosa);
    p.p2 = rotate_point_z(p.p2,sina, cosa);
    p.p3 = rotate_point_z(p.p3,sina, cosa);
}*/

/*
QImage render(Camera *cam, Light *lght, ObjectList *LIST)
{
    QImag
e image(800,600,QImage::Format_RGB32);
    QRgb colour;
    colour = qRgb(0,0,0);

    for(int i = 0; i < 800; i++)
        for(int j = 0; i < 600; i++)
            image.setPixel(i, j, colour);

    return image;
}
*/
