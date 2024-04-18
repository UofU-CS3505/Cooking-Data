#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>

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
    bool isGamePaused = false;
    bool isStartMenu = true;
    int selectedObjectIndex;

    float const SCALE = 10.0f;

    void createBody(float x, float y, float halfWidth, float halfHeight, double angle);

public:
    Interface(QWidget *parent = nullptr);
    ~Interface();

private slots:
    void displayHelpPopup();

public slots:
    void updateObject(int index, const b2Body* source);
    void createGround(b2Vec2 loc, int width, int height);
    // WE WILL CHANGE THIS LATER
    void startLevel();
    void openStartMenu();

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    void draggableClicked(int index, int x, int y);
    void draggableReleased();
    void escPressed(bool pauseState);

};
#endif // INTERFACE_H
