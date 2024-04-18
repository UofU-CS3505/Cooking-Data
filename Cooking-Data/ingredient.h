#ifndef INGREDIENT_H
#define INGREDIENT_H

#include "IngredientType.h"

#include <QLabel>
#include <QPoint>
#include <QSize>

class Ingredient {
private:
    QPoint position;

    ///
    /// \brief dimensions The dimension of the ingredient in inches. Should be
    ///                   half of the dimension of the texture.
    ///
    QSize dimensions;

    ///
    /// \brief orientation The orientation of the ingredient in degrees.
    ///
    double orientation;

    ///
    /// \brief texture The dimension of the texture should be double that of
    ///                the Ingredient's dimension.
    ///
    QPixmap texture;

public:

    const IngredientType INGREDIENT_TYPE;

    Ingredient();
    Ingredient(IngredientType type, QPoint pos, QSize size, double angle, QPixmap texture);
    ~Ingredient();

    QPoint getPosition() const;
    QSize getDimensions() const;
    double getOrientation() const;
    QPixmap getTexture() const;
    double getRadius() const;

    void setPosition(QPoint location);
    void setDimensions(QSize size);
    void setOrientation(double angle);

};

#endif // INGREDIENT_H
