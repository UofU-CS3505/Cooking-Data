#include "ingredient.h"
#include "ingredienttype.h"

int Ingredient::globalLatestID = 0;

Ingredient::Ingredient():
    ID(++globalLatestID),
    INGREDIENT_TYPE(None),
    dimensions(QSizeF(0.5,0.5)),
    weight(1),
    texture(QPixmap(":/ingredients/assets/images/sprites/Error.png")),
    position(QPointF(0,0)),
    angle(0) {

}

Ingredient::Ingredient(QPointF position):
    ID(++globalLatestID),
    INGREDIENT_TYPE(None),
    dimensions(QSizeF(0.5,0.5)),
    weight(1),
    texture(QPixmap(":/ingredients/assets/images/sprites/Error.png")),
    position(position),
    angle(0) {

}

Ingredient::Ingredient(IngredientType type, QSizeF dimension, double weight, QPixmap texture, QPointF position, double angle):
    ID(++globalLatestID),
    INGREDIENT_TYPE(type),
    dimensions(dimension),
    weight(weight),
    texture(texture),
    position(position),
    angle(angle) {
    qDebug() << "Creating Ingredient with ID" << ID;
}

Ingredient::~Ingredient() {
    // QPixmap handles its own memory.
}

Ingredient::Ingredient(const Ingredient& rhs):
    ID(++globalLatestID),
    INGREDIENT_TYPE(rhs.INGREDIENT_TYPE),
    dimensions(rhs.dimensions),
    weight(rhs.weight),
    texture(rhs.texture),
    position(rhs.position),
    angle(rhs.angle) {

}

Ingredient& Ingredient::operator=(const Ingredient& other) {
    if (this != &other) { // protect against self-assignment
        // Use a new ID as these are two different Ingredient instances.
        ID = ++globalLatestID;

        // Copy all fields.
        INGREDIENT_TYPE = other.INGREDIENT_TYPE;
        dimensions = other.dimensions;
        weight = other.weight;
        texture = other.texture; // QPixmap's assignment operator handles deep copy
        position = other.position;
        angle = other.angle;
    }

    return *this; // return this object by reference
}

bool Ingredient::operator==(const Ingredient& rhs) const {
    return ID == rhs.ID;
}

QPixmap Ingredient::getTexture() const {
    if (texture.isNull())
        return QPixmap(":/ingredients/assets/images/sprites/Error.png");

    return texture;
}
