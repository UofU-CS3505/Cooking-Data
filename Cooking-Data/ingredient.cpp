#include "ingredient.h"

Ingredient::Ingredient():
    position(QPoint(0,0)),
    dimensions(QSize(10,10)),
    orientation(0){}

Ingredient::Ingredient(QPoint pos, QSize size, double angle):
    position(pos),
    dimensions(size),
    orientation(angle){}

Ingredient::~Ingredient() {

}

QPoint Ingredient::getPosition() {
    return position;
}

QSize Ingredient::getDimensions() {
    return dimensions;
}

double Ingredient::getOrientation() {
    return orientation;
}

void Ingredient::setPosition(QPoint location) {
    position = QPoint(location);
}

void Ingredient::setDimensions(QSize size) {
    dimensions = QSize(size);
}

void Ingredient::setOrientation(double angle) {
    orientation = angle;
}
