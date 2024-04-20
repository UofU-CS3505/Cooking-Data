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
    // QVector<QPair<QPixmap, Ingredient>> sprites;
    QVector<QLabel*> bodyDisplays;
    bool mouseIsDown;
    bool isGamePaused = false;
    bool isStartMenu = true;
    int selectedObjectIndex;

    float const SCALE = 10.0f;

    ///
    /// \brief createBody
    /// \param x the x position, 0 is left and positive is right
    /// \param y the y position, 0 is top and positive is down
    /// \param width the width
    /// \param height the height
    /// \param angle the angle in degrees
    ///
    // void createBody(float x, float y, float width, float height, float angle);

public:
    Interface(QWidget *parent = nullptr);
    ~Interface();

private slots:
    void displayHelpPopup();

public slots:
    void createLabels(QVector<Ingredient> ingredients);
    void updateObject(int index, Ingredient ingredient);
    void createGround(b2Vec2 loc, int width, int height);
    // WE WILL CHANGE THIS LATER
    void startLevel();
    void openStartMenu();

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    void createWorld();
    void deleteWorld();
    void mousePressed(QPointF position);
    void mouseMoved(QPointF position);
    void mouseReleased();
    void escPressed(bool pauseState);

};
#endif // INTERFACE_H
