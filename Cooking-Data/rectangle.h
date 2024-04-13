#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QLabel>
#include <QPoint>
#include <QSize>

class Rectangle
{
public:
    Rectangle();
    Rectangle(QPoint pos, QSize size, double angle);
    ~Rectangle();

    QPoint getPosition();
    QSize getDimensions();
    double getOrientation();

    void setPosition(QPoint location);
    void setDimensions(QSize size);
    void setOrientation(double angle);

private:
    QPoint position;
    QSize dimensions;
    double orientation;
};

#endif // RECTANGLE_H
