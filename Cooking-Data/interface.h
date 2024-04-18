#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>

#include "model.h"
#include "ingredient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Interface;
}
QT_END_NAMESPACE

class Interface : public QMainWindow {
    Q_OBJECT
private:
    Ui::Interface *ui;
    Model model;
    QVector<QPair<QPixmap, Ingredient>> sprites;
    QVector<QLabel*> bodyDisplays;
    bool mouseIsDown;
    int selectedObjectIndex;

    float const SCALE = 10.0f;

    void createBody(float x, float y, float halfWidth, float halfHeight, double angle);

public:
    Interface(QWidget *parent = nullptr);
    ~Interface();

public slots:
    void updateObject(int index, const b2Body* source);
    void createGround(b2Vec2 loc, int width, int height);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    void draggableClicked(int index, int x, int y);
    void draggableReleased();

};
#endif // INTERFACE_H
