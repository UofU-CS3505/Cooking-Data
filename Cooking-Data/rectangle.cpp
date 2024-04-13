#include "rectangle.h"

Rectangle::Rectangle():
    position(QPoint(0,0)),
    dimensions(QSize(10,10)),
    orientation(0){}

Rectangle::Rectangle(QPoint pos, QSize size, double angle):
    position(pos),
    dimensions(size),
    orientation(angle){}

Rectangle::~Rectangle(){

}


QPoint Rectangle::getPosition() {
    return position;
}
QSize Rectangle::getDimensions(){
    return dimensions;
}
double Rectangle::getOrientation(){
    return orientation;
}

void Rectangle::setPosition(QPoint location){
    position = QPoint(location);
}
void Rectangle::setDimensions(QSize size){
    dimensions = QSize(size);
}
void Rectangle::setOrientation(double angle){
    orientation = angle;
}
