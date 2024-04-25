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
#include <QtMultimedia/QtMultimedia>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Interface;
}
QT_END_NAMESPACE

class Interface : public QMainWindow {
    Q_OBJECT
private:
    const double SCALE = 400;
    const double TEXTURE_SCALE = 400.0 / 72;

    ///
    /// \brief comboPlayer Handles the sound effect player
    ///
    QSoundEffect comboPlayer;
    ///
    /// \brief backgroundPlayer Handles the background music
    ///
    QSoundEffect backgroundPlayer;

    ///
    /// \brief ui The UI
    ///
    Ui::Interface *ui;
    ///
    /// \brief model Stores all of the logic
    ///
    Model model;
    ///
    /// \brief graphicsScene Displays sprites representing the bodies
    ///
    QGraphicsScene graphicsScene = QGraphicsScene(0, 0, 790, 550);

    ///
    /// \brief mouseIsDown Holds status of the mouse
    ///
    bool mouseIsDown;
    ///
    /// \brief isGamePaused Holds status of timer
    ///
    bool isGamePaused = false;
    ///
    /// \brief isStartMenu Holds status if page is in main manu
    ///
    bool isStartMenu = true;
    ///
    /// \brief currentLevel Holds what level it is
    ///
    int currentLevel = 0;
    ///
    /// \brief level1Done Determines if level 2 should be available
    ///
    bool level1Done = false;
    ///
    /// \brief level2Done Determines if level 3 should be available
    ///
    bool level2Done = false;
    ///
    /// \brief displayRecipeText Display the recipe label
    ///
    void displayRecipeText();

public:
    ///
    /// \brief Interface
    /// \param parent
    ///
    Interface(QWidget *parent = nullptr);
    ~Interface();

private slots:
    ///
    /// \brief startLevel Load the setup for a level
    ///
    void startLevel();
    ///
    /// \brief completeLevel Display victory text and bring user back to menu
    ///
    void completeLevel();
    ///
    /// \brief openStartMenu Opens the main menu, resets everything else
    ///
    void openStartMenu();
    ///
    /// \brief displayHelpPopup Open tutorial popup
    ///
    void displayHelpPopup();

    ///
    /// \brief mousePressEvent Check for mouse clicks
    /// \param event
    ///
    void mousePressEvent(QMouseEvent* event) override;
    ///
    /// \brief mouseMoveEvent Check for mouse movements
    /// \param event
    ///
    void mouseMoveEvent(QMouseEvent* event) override;
    ///
    /// \brief mouseReleaseEvent Check for mouse being released
    /// \param event
    ///
    void mouseReleaseEvent(QMouseEvent* event) override;
    ///
    /// \brief keyPressEvent Check for ESC and R presses
    /// \param event
    ///
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    ///
    /// \brief beginFrame Reset the graphic scene to draw next frame
    ///
    void beginFrame();
    ///
    /// \brief addIngredientToFrame Draw the ingredient to the scene
    /// \param ingredient
    ///
    void addIngredientToFrame(const Ingredient& ingredient);
    ///
    /// \brief endFrame Display the graphic scene
    ///
    void endFrame();
    ///
    /// \brief playSound Play the combo sound effect
    ///
    void playSound();

signals:
    ///
    /// \brief createWorld Tell the model that a level should be made
    /// \param level int to determine which level to load
    ///
    void createWorld(int level);
    ///
    /// \brief deleteWorld Tell the model to remove everything from the world
    ///
    void deleteWorld();
    ///
    /// \brief mousePressed Tell the model that a mouse was pressed
    /// \param position The position of the mouse click
    ///
    void mousePressed(QPointF position);
    ///
    /// \brief mouseMoved Tell the model that a mouse was moved
    /// \param position The position of the mouse movement
    ///
    void mouseMoved(QPointF position);
    ///
    /// \brief mouseReleased Tell the model that the mouse was relesed
    ///
    void mouseReleased();
    ///
    /// \brief escPressed Tell the model that Escape was pressed
    /// \param pauseState Determines if the
    ///
    void escPressed(bool pauseState);

};
#endif // INTERFACE_H
