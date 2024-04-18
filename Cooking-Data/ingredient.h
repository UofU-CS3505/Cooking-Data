#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QLabel>
#include <QPoint>
#include <QSize>

class Ingredient {
private:
    QPointF position;

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
    Ingredient();
    Ingredient(QPointF position, QSize dimension, double angle, QPixmap texture);
    ~Ingredient();

    QPointF const getPosition();
    QSize const getDimensions();

    ///
    /// \brief getOrientation Get the orientation in degrees.
    /// \return the orientation in degrees
    ///
    double getOrientation();
    QPixmap const getTexture();
    double getRadius();

    void setPosition(QPointF newPositions);
    void setDimensions(QSize newDimensions);
    void setOrientation(double angle);

};

#endif // INGREDIENT_H
