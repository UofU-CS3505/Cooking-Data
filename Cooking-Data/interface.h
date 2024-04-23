#ifndef INTERFACE_H
#define INTERFACE_H

#include <QGraphicsScene>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>

#include "model.h"
#include "ingredient.h"
#include "qgraphicsscene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Interface;
}
QT_END_NAMESPACE

class Interface : public QMainWindow {
    Q_OBJECT
private:
    const float SCALE = 500.0f;

    Ui::Interface *ui;
    Model model;
    QGraphicsScene graphicsScene = QGraphicsScene(0, 0, 790, 550);

    bool mouseIsDown;
    bool isGamePaused = false;
    bool isStartMenu = true;
    int currentLevel = 0;

public:
    Interface(QWidget *parent = nullptr);
    ~Interface();

private slots:
    void displayHelpPopup();

public slots:
    void beginFrame();
    void addIngredientToFrame(const Ingredient& ingredient);
    void endFrame();
    void createGround(b2Vec2 loc, int width, int height);
    // WE WILL CHANGE THIS LATER
    void startLevel();
    void openStartMenu();

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
signals:
    void createWorld(int level);
    void deleteWorld();
    void mousePressed(QPointF position);
    void mouseMoved(QPointF position);
    void mouseReleased();
    void escPressed(bool pauseState);

};
#endif // INTERFACE_H
