#include "ingredient.h"

Ingredient::Ingredient():
    position(QPointF(0,0)),
    dimensions(QSize(10,10)),
    orientation(0) {}

Ingredient::Ingredient(QPointF position, QSize dimension, double angle, QPixmap texture):
    position(position),
    dimensions(dimension),
    orientation(angle),
    texture(texture) {}

Ingredient::~Ingredient() {

}

QPointF const Ingredient::getPosition() {
    return position;
}

QSize const Ingredient::getDimensions() {
    return dimensions;
}

double Ingredient::getOrientation() {
    return orientation;
}

QPixmap const Ingredient::getTexture() {
    return texture;
}

double Ingredient::getRadius() {
    return std::max(dimensions.width(), dimensions.height());
}

void Ingredient::setPosition(QPointF newPositions) {
    position = newPositions;
}

void Ingredient::setDimensions(QSize newDimensions) {
    dimensions = newDimensions;
}

void Ingredient::setOrientation(double angle) {
    orientation = angle;
}
