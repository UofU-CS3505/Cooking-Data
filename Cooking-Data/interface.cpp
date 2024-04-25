#include <QLayout>

#include <QGraphicsItem>

#include "interface.h"
#include "model.h"
#include "ui_interface.h"

Interface::Interface(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Interface),
    model(Model()) {
    ui->setupUi(this);

    // Sound effects.
    comboPlayer.setSource(QUrl::fromLocalFile(
        ":/soundEffects/assets/sounds/Combo.wav"));
    backgroundPlayer.setSource(QUrl::fromLocalFile(
        ":/songs/assets/sounds/CookingDataBackground.wav"));

    comboPlayer.setVolume(0.25f);
    backgroundPlayer.setVolume(0.20f);

    comboPlayer.setLoopCount(0);
    backgroundPlayer.setLoopCount(QSoundEffect::Infinite);

    backgroundPlayer.play();
    // Connect the combo player to the combination completed signal from the
    // model.
    connect(&model, &Model::comboCompleted,
            &comboPlayer, &QSoundEffect::play);

    ui->pauseLabel->setVisible(false);
    ui->recipeLabel->setVisible(false);
    ui->winLabel->setVisible(false);
    ui->quitButton->setVisible(false);
    ui->controlsButton->setVisible(false);

    // Connect world creation and deletion to the view.
    connect(this, &Interface::createWorld,
            &model, &Model::createWorld);
    connect(this, &Interface::deleteWorld,
            &model, &Model::deleteWorld);

    // Connect game updates to the view.
    connect(&model, &Model::frameBegan,
            this, &Interface::beginFrame);
    connect(&model, &Model::ingredientUpdated,
            this, &Interface::addIngredientToFrame);
    connect(&model, &Model::frameEnded,
            this, &Interface::endFrame);
    connect(&model, &Model::winConditionMet,
            this, &Interface::completeLevel);

    // Connect mouse updates to model.
    connect(this, &Interface::mousePressed,
            &model, &Model::pointPressed);
    connect(this, &Interface::mouseMoved,
            &model, &Model::pointMoved);
    connect(this, &Interface::mouseReleased,
            &model, &Model::pointReleased);

    // Connect keyboard updates to model.
    connect(this, &Interface::escPressed,
            &model, &Model::pauseGame);

    // Connect menu buttons.
    connect(ui->controlsButton, &QPushButton::clicked,
            this, &Interface::displayHelpPopup);
    connect(ui->tutorialButton, &QPushButton::clicked,
            this, &Interface::displayHelpPopup);
    connect(ui->level1, &QPushButton::clicked,
            this, [&](){currentLevel = 1;
                        startLevel();});
    connect(ui->level2, &QPushButton::clicked,
            this, [&](){currentLevel = 2;
                        startLevel();});
    connect(ui->level3, &QPushButton::clicked,
            this, [&](){currentLevel = 3;
                        startLevel();});
    connect(ui->quitButton, &QPushButton::clicked,
            this, &Interface::openStartMenu);

    // Put the QGraphicsScene into* the QGraphicsView.
    ui->graphicsView->setScene(&graphicsScene);

    // Set up the start screen.
    QPixmap cookingData = QPixmap(
        ":/ingredients/assets/images/sprites/CookingData.png");
    cookingData = cookingData.scaled(ui->cookingDataLabel->width(), ui->cookingDataLabel->height(),
                             Qt::KeepAspectRatio);
    ui->cookingDataLabel->setPixmap(cookingData);
    ui->startWidget->raise();
    ui->level1->raise();
    // For reasons unknown, the style sheet of the top level widget does not
    // apply to these.
    ui->startWidget->setStyleSheet(
        "QWidget{background-color : rgba(200, 200, 200, 80); color : black;}"
        "QAbstractButton {"
        "padding: 4px;border-radius: 4px;background-color: #ff8000;}"
        "QAbstractButton:hover {background-color: #ffa010;}"
        "QAbstractButton:pressed {background-color: #d07000;}"
        "QAbstractButton:disabled {background-color: #303030; color: #808080}");
    emit escPressed(true);
    ui->escLabel->setVisible(false);
}

Interface::~Interface() {
    delete ui;
}

void Interface::displayRecipeText() {
    if (ui->recipeLabel->isVisible()) {
        ui->recipeLabel->setVisible(false);
    } else {
        ui->recipeLabel->setVisible(true);
        ui->recipeLabel->raise();
    }
}

void Interface::startLevel() {
    emit escPressed(false);
    isStartMenu = false;
    ui->stackedWidget->setCurrentIndex(1);
    ui->escLabel->setVisible(true);
    ui->escLabel->raise();
    ui->recipeLabel->setVisible(true);
    ui->recipeLabel->raise();
    ui->startWidget->setEnabled(false);
    ui->startWidget->setVisible(false);

    if (currentLevel == 1)
        ui->recipeLabel->setText(
            "OATMEAL RECIPE \n"
            "1 - Boil water in a pot on  the stove. \n"
            "2 - Add a packet of oatmeal into a bowl. \n"
            "3 - After water is boiling, add it to the bowl using a ladle. \n"
            "4 - Congrats! You made oatmeal! \n\n"
            "WARNING: Leaving the stove on may cause fires! \n"
            "Press \"R\" at anytime to Show/Hide the recipe.");
    if (currentLevel == 2)
        ui->recipeLabel->setText(
            "SANDWICH RECIPE \n"
            "1 - Cut some slices of bread.\n"
            "2 - Cut a tomato and add it to a piece of bread.\n"
            "3 - Cut some ham and add it to the sandwich.\n"
            "4 - Cut some lettuce and add that as well.\n"
            "5 - Finally, top off the sandwich with another piece of \n "
            "bread. \n"
            "6 - Congrats! You made a ham sandwich! \n\n"
            "WARNING: Knives can be dangerous. Be careful! \n"
            "Press \"R\" at anytime to Show/Hide the recipe.");
    if (currentLevel == 3)
        ui->recipeLabel->setText(
            "CHICKEN, BROCCOLI, AND RICE RECIPE \n"
            "1 - Prepare and cut some chicken.\n"
            "2 - Put chicken on the stove in a pan to cook.\n"
            "3 - Boil some rice in a pot.\n"
            "4 - Cut and prepare some broccoli.\n"
            "5 - Add the rice and chicken to a plate.\n"
            "6 - Finish the dish by adding the broccoli. \n"
            "7 - Congrats! You made some Chicken, Broccoli, \n"
            "and Rice!\n\n"
            "WARNING: Leaving the stove on may cause fires! \n"
            "WARNING: Knives can be dangerous. Be careful! \n"
            "Press \"R\" at anytime to Show/Hide the recipe.");

    // Grab mouse to consume all mouse events, as otherwise the
    // qGraphicsView/Scene takes all of it.
    this->grabMouse();

    emit createWorld(currentLevel);
}

void Interface::completeLevel() {
    ui->escLabel->setVisible(false);
    ui->escLabel->raise();
    ui->winLabel->setVisible(true);
    ui->winLabel->raise();

    // Grab mouse to consume all mouse events, as otherwise the
    // qGraphicsView/Scene takes all of it.
    this->grabMouse();

    if (currentLevel == 1)
        level1Done = true;
    if (currentLevel == 2)
        level2Done = true;

    QTimer::singleShot(1500, this, &Interface::openStartMenu);
}

void Interface::openStartMenu() {
    if (isGamePaused)
        isGamePaused = !isGamePaused;
    ui->pauseLabel->setVisible(isGamePaused);
    ui->quitButton->setVisible(isGamePaused);
    ui->quitButton->setEnabled(isGamePaused);
    ui->controlsButton->setVisible(isGamePaused);
    ui->controlsButton->setEnabled(isGamePaused);
    ui->escLabel->setVisible(!isGamePaused);
    ui->escLabel->raise();
    ui->winLabel->setVisible(false);
    ui->recipeLabel->setVisible(false);

    emit deleteWorld();

    isStartMenu = true;

    ui->level2->setEnabled(level1Done);
    ui->level3->setEnabled(level2Done);

    // Release mouse to stop consume all mouse events.
    this->releaseMouse();

    ui->stackedWidget->setCurrentIndex(0);
    ui->startWidget->setEnabled(true);
    ui->startWidget->setVisible(true);
    ui->startWidget->raise();
    ui->startWidget->raise();
    ui->escLabel->setVisible(false);
}

void Interface::displayHelpPopup() {
    QMessageBox brushHelp;
    brushHelp.setWindowTitle("Tutorials");
    brushHelp.setText(
        "Drag objects around with your mouse to move them around the screen.\n"
        "Press R to show or hide the recipe.\n"
        "Follow the recipe given to you to cook the meal and progress.\n"
        "Completing a recipe will give you a new recipe to cook.\n"
        "Click the stove to turn it on and off.\n"
        "The pot and pan should go on the stove.\n"
        "The knife, ladel, and pitcher must be being dragged to cut or pour.\n"
        "If you burn something out of existence, quit to menu and try again.\n"
        "You already know this, but press ESC to pause.");
    brushHelp.setIcon(QMessageBox::Information);
    brushHelp.setModal(true);
    brushHelp.setDefaultButton(QMessageBox::Ok);
    brushHelp.exec();
}

void Interface::mouseMoveEvent(QMouseEvent* event) {
    // qDebug() << event->pos().x() << " | " << event->pos().y();

    // if (mouseIsDown)
    emit mouseMoved(QPointF(event->pos().x() * 1.0 / SCALE,
                            event->pos().y() * 1.0 / SCALE));
}

void Interface::mousePressEvent(QMouseEvent* event) {
    // qDebug() << event->pos().x() << " | " << event->pos().y();

    mouseIsDown = true;
    emit mousePressed(QPointF(event->pos().x() * 1.0 / SCALE,
                              event->pos().y() * 1.0 / SCALE));
}

void Interface::mouseReleaseEvent(QMouseEvent* event) {
    mouseIsDown = false;
    emit mouseReleased();
}

void Interface::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_F1) {
        level1Done = true;
        level2Done = true;
        openStartMenu();
        return;
    }

    if (isStartMenu)
        return;

    if (event->key() == Qt::Key_R) {
        displayRecipeText();
        return;
    }

    if (event->key() == Qt::Key_Escape) {
        isGamePaused = !isGamePaused;
        emit escPressed(isGamePaused);

        if (isGamePaused)
            // Release mouse to stop consume all mouse events.
            this->releaseMouse();
        else
            // Grab mouse to consume all mouse events, as otherwise the
            // qGraphicsView/Scene takes all of it.
            this->grabMouse();

        ui->pauseLabel->setVisible(isGamePaused);
        ui->pauseLabel->raise();
        ui->quitButton->setVisible(isGamePaused);
        ui->quitButton->setEnabled(isGamePaused);
        ui->quitButton->raise();
        ui->controlsButton->setVisible(isGamePaused);
        ui->controlsButton->setEnabled(isGamePaused);
        ui->controlsButton->raise();
        ui->escLabel->setVisible(!isGamePaused);
        ui->escLabel->raise();
        return;
    }
}

void Interface::beginFrame() {
    graphicsScene.clear();

    // Rebuild background at frame reset
    QPixmap background = QPixmap(
        ":/ingredients/assets/images/sprites/Background.png");

    for (int y = 0; y <= this->height() / TEXTURE_SCALE / background.height(); y++) {
        for (int x = 0; x <= this->width() / TEXTURE_SCALE / background.width(); x++) {
            QGraphicsPixmapItem* bg = graphicsScene.addPixmap(background);
            bg->setPos(x * background.width() * TEXTURE_SCALE,
                       y * background.height() * TEXTURE_SCALE);
            bg->setScale(TEXTURE_SCALE);
            bg->setRotation(0);
        }
    }

    QPixmap table = QPixmap(
        ":/ingredients/assets/images/sprites/Table.png");
    // 1/5 of the screen
    float tableWidth = graphicsScene.width() / 5;
    for (int i = 1; i <= 5; i++) {
        QGraphicsPixmapItem* floor = graphicsScene.addPixmap(table);
        floor->setPos(graphicsScene.width() - tableWidth * i, 500);
        floor->setScale(tableWidth / table.width());
        floor->setRotation(0);
    }

    QPixmap windowSprite = QPixmap(
        ":/ingredients/assets/images/sprites/Window.png");
    QGraphicsPixmapItem* window = graphicsScene.addPixmap(windowSprite);
    window->setPos(25 * TEXTURE_SCALE, 23 * TEXTURE_SCALE);
    window->setScale(TEXTURE_SCALE);
    window->setRotation(0);
}

void Interface::addIngredientToFrame(const Ingredient &ingredient) {
    // qDebug() << "Drawing ingredient ID" << ingredient.getID();
    double x = ingredient.getPosition().x() * SCALE;
    double y = ingredient.getPosition().y() * SCALE;

    double angle = ingredient.getAngle();

    QGraphicsPixmapItem* item = graphicsScene.addPixmap(ingredient.getTexture());
    // Assumes the size of the Ingredient is half of its texture size.
    item->setOffset(
        -ingredient.getTexture().width() / 2,
        -ingredient.getTexture().height() / 2);
    item->setPos(x, y);
    // The Pixmaps are in the scale of 2 pixels per inch, but b2Body uses
    // meters as units.
    // I have no idea why this specific number works. It just does.
    item->setScale(TEXTURE_SCALE);
    item->setRotation(angle);
}

void Interface::endFrame() {
    ui->graphicsView->show();
}
