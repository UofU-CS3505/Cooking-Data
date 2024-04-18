#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QLabel>
#include <QPoint>
#include <QSize>

class Ingredient {
private:
    QPoint position;
    QSize dimensions;
    double orientation;

public:
    Ingredient();
    Ingredient(QPoint pos, QSize size, double angle);
    ~Ingredient();

    QPoint getPosition();
    QSize getDimensions();
    double getOrientation();

    void setPosition(QPoint location);
    void setDimensions(QSize size);
    void setOrientation(double angle);

};

#endif // INGREDIENT_H
