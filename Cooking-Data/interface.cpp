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

    ui->pauseLabel->setStyleSheet("background-color : rgba(200, 200, 200, 150); color : black;");
    ui->escLabel->setStyleSheet("color : black;");
    ui->pauseLabel->setVisible(false);
    ui->quitButton->setVisible(false);
    ui->controlsButton->setVisible(false);

    // Connect world creation and deletion to the view.
    connect(this, &Interface::createWorld,
            &model, &Model::createWorld);
    connect(this, &Interface::deleteWorld,
            &model, &Model::deleteWorld);

    // Connect game updates to the view
    connect(&model, &Model::makeGroundInView,
            this, &Interface::createGround);
    connect(&model, &Model::frameBegan,
            this, &Interface::beginFrame);
    connect(&model, &Model::ingredientUpdated,
            this, &Interface::addIngredientToFrame);
    connect(&model, &Model::frameEnded,
            this, &Interface::endFrame);

    // Connect mouse updates to model
    connect(this, &Interface::mousePressed,
            &model, &Model::pointPressed);
    connect(this, &Interface::mouseMoved,
            &model, &Model::pointMoved);
    connect(this, &Interface::mouseReleased,
            &model, &Model::pointReleased);

    // Connect keyboard updates to model
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
    ui->startWidget->setStyleSheet("QWidget{background-color : rgba(200, 200, 200, 80); color : black;}"
                                   "QAbstractButton {"
                                   "padding: 4px;border-radius: 4px;background-color: #ff8000;}"
                                   "QAbstractButton:hover {background-color: #ffa010;}"
                                   "QAbstractButton:pressed {background-color: #d07000;}"
                                   "QAbstractButton:disabled {background-color: #303030; color: #808080}");
    emit escPressed(true);
    ui->escLabel->setVisible(false);
}

void Interface::displayHelpPopup() {
    QMessageBox brushHelp;
    brushHelp.setWindowTitle("Tutorials");
    brushHelp.setText("Drag objects around with your mouse to move them around the screen. \n"
                      "Follow the recipe given to you to cook the meal and progress. \n"
                      "Completing a recipe will give you a new recipe to cook. \n"
                      "Click the pantry button to see available ingredients. \n"
                      "The pot should go on the stove. Press dial to turn it on. \n"
                      "You already know this, but press ESC to pause.");
    brushHelp.setIcon(QMessageBox::Information);
    brushHelp.setModal(true);
    brushHelp.setDefaultButton(QMessageBox::Ok);
    brushHelp.exec();
}

void Interface::beginFrame() {
    graphicsScene.clear();

    // Rebuild background at frame reset
    QPixmap wood = QPixmap(
        ":/ingredients/assets/images/sprites/Background.png");
    QGraphicsPixmapItem* bg = graphicsScene.addPixmap(wood);
    bg->setPos(0, 0);
    bg->setScale(13);
    bg->setRotation(0);

    QPixmap table = QPixmap(
        ":/ingredients/assets/images/sprites/Table.png");
    QGraphicsPixmapItem* floor = graphicsScene.addPixmap(table);
    // 3/5 of the screen
    float tableWidth = graphicsScene.width() / 5 * 3;
    floor->setPos(graphicsScene.width() - tableWidth, 500);
    floor->setScale(tableWidth / table.width());
    floor->setRotation(0);

    QPixmap windowSprite = QPixmap(
        ":/ingredients/assets/images/sprites/Window.png");
    QGraphicsPixmapItem* window = graphicsScene.addPixmap(windowSprite);
    window->setPos(50, 50);
    window->setScale(8);
    window->setRotation(0);
}

void Interface::addIngredientToFrame(Ingredient& ingredient) {
    // BELOW IS TEMP FOR A TEST, BUT MAY WORK AS A BASE
    // Ok, so right now the box doesn't have the acurate size because it is
    // impossibly complicated to get a bodies size for some reason
    // So, we are making a different class to store a sprite's info right?
    // Put the size in there

    // qDebug() << "Drawing ingredient ID" << ingredient.getID();
    double x = ingredient.getPosition().x() * SCALE;
    double y = ingredient.getPosition().y() * SCALE;

    double angle = ingredient.getAngle();

    QGraphicsPixmapItem* item = graphicsScene.addPixmap(ingredient.getTexture());
    item->setOffset(
        -ingredient.getDimensions().width(),
        -ingredient.getDimensions().height());
    item->setPos(x, y);
    item->setScale(SCALE / 2);
    item->setRotation(angle);
}

void Interface::endFrame() {
    ui->graphicsView->show();
}

void Interface::createGround(b2Vec2 loc, int width, int height) {
    // Temp until we store other objects
    // ui->ground->setGeometry(loc.x * SCALE, loc.y * SCALE - height * SCALE/2,
    //                         width * SCALE, height * SCALE);
    // ui->ground->setStyleSheet(
    //     "QLabel { background-color : brown; color : black; }");
}

void Interface::startLevel() {
    emit escPressed(false);
    isStartMenu = false;
    ui->stackedWidget->setCurrentIndex(1);
    ui->escLabel->setVisible(true);
    ui->escLabel->raise();
    ui->startWidget->setEnabled(false);
    ui->startWidget->setVisible(false);

    // Grab mouse to consume all mouse events, as otherwise the
    // qGraphicsView/Scene takes all of it.
    this->grabMouse();

    emit createWorld(currentLevel);
}

void Interface::openStartMenu() {
    isGamePaused = !isGamePaused;
    ui->pauseLabel->setVisible(isGamePaused);
    ui->quitButton->setVisible(isGamePaused);
    ui->quitButton->setEnabled(isGamePaused);
    ui->controlsButton->setVisible(isGamePaused);
    ui->controlsButton->setEnabled(isGamePaused);
    ui->escLabel->setVisible(!isGamePaused);
    ui->escLabel->raise();

    emit deleteWorld();

    isStartMenu = true;

    // Release mouse to stop consume all mouse events.
    this->releaseMouse();

    ui->stackedWidget->setCurrentIndex(0);
    ui->startWidget->setEnabled(true);
    ui->startWidget->setVisible(true);
    ui->startWidget->raise();
    ui->startWidget->raise();
    ui->escLabel->setVisible(false);
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
    if (isStartMenu)
        return;
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
    }
}

Interface::~Interface() {
    delete ui;
}
