#include "ingredient.h"
#include "IngredientType.h"

Ingredient::Ingredient():
    position(QPoint(0,0)),
    dimensions(QSize(10,10)),
    orientation(0),
    INGREDIENT_TYPE(None) {}

Ingredient::Ingredient(IngredientType type, QPoint pos, QSize size, double angle, QPixmap texture):
    position(pos),
    dimensions(size),
    orientation(angle),
    texture(texture),
    INGREDIENT_TYPE(type) {}

Ingredient::~Ingredient() {

}

QPoint Ingredient::getPosition() const {
    return position;
}

QSize Ingredient::getDimensions() const {
    return dimensions;
}

double Ingredient::getOrientation() const {
    return orientation;
}

QPixmap Ingredient::getTexture() const {
    return texture;
}

double Ingredient::getRadius() const {
    return std::max(dimensions.width(), dimensions.height());
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
