#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QLabel>
#include <QPoint>
#include <QSize>

class Rectangle {
private:
    QPoint position;
    QSize dimensions;
    double orientation;

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

};

#endif // RECTANGLE_H
