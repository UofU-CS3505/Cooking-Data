#include "ingredient.h"
#include "ingredienttype.h"

int Ingredient::globalLatestID = 0;

Ingredient::Ingredient():
    ID(++globalLatestID),
    INGREDIENT_TYPE(None),
    position(QPointF(0,0)),
    dimensions(QSize(10,10)),
    angle(0) {

}

Ingredient::Ingredient(IngredientType type, QPointF position, QSizeF dimension, double angle, QPixmap texture):
    ID(++globalLatestID),
    INGREDIENT_TYPE(type),
    position(position),
    dimensions(dimension),
    angle(angle),
    texture(texture) {
    qDebug() << "Creating Ingredient with ID" << ID;
}

Ingredient::~Ingredient() {
    // QPixmap handles its own memory.
}

Ingredient::Ingredient(const Ingredient& rhs):
    ID(++globalLatestID),
    INGREDIENT_TYPE(rhs.INGREDIENT_TYPE),
    position(rhs.position),
    dimensions(rhs.dimensions),
    angle(rhs.angle),
    texture(rhs.texture) {

}

Ingredient& Ingredient::operator=(const Ingredient& other) {
    if (this != &other) { // protect against self-assignment
        // Use a new ID as these are two different Ingredient instances.
        ID = ++globalLatestID;

        // Copy all fields.
        INGREDIENT_TYPE = other.INGREDIENT_TYPE;
        position = other.position;
        dimensions = other.dimensions;
        angle = other.angle;
        texture = other.texture; // QPixmap's assignment operator handles deep copy
    }

    return *this; // return this object by reference
}

bool Ingredient::operator==(const Ingredient& rhs) const {
    return ID == rhs.ID;
}

double Ingredient::getRadius() const {
    return std::max(dimensions.width(), dimensions.height()) / 2.0;
}
