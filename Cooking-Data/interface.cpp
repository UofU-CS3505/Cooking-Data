#include <QLayout>
#include <iostream>
#include <ostream>

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

    // Connect world creation to the view.
    connect(this, &Interface::createWorld,
            &model, &Model::createWorld);
    connect(&model, &Model::worldCreated,
            this, &Interface::createLabels);

    // Connect game updates to the view
    connect(&model, &Model::makeGroundInView,
            this, &Interface::createGround);
    connect(&model, &Model::objectUpdated,
            this, &Interface::updateObject);

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

    // Connect menu buttons
    connect(ui->controlsButton, &QPushButton::clicked,
            this, &Interface::displayHelpPopup);
    connect(ui->tutorialButton, &QPushButton::clicked,
            this, &Interface::displayHelpPopup);
    connect(ui->level1, &QPushButton::clicked,
            this, &Interface::startLevel);
    connect(ui->quitButton, &QPushButton::clicked,
            this, &Interface::openStartMenu);

    QPixmap cookingData = QPixmap(
        ":/ingredients/assets/images/sprites/CookingData.png");
    cookingData = cookingData.scaled(ui->cookingDataLabel->width(), ui->cookingDataLabel->height(),
                             Qt::KeepAspectRatio);
    ui->cookingDataLabel->setPixmap(cookingData);
    ui->startWidget->raise();
    ui->level1->raise();
    ui->startWidget->setStyleSheet("QWidget{background-color : rgba(200, 200, 200, 80); color : black;}"
                                   "QAbstractButton {"
                                   "padding: 4px;border-radius: 4px;background-color: #ff8000;}"
                                   "QAbstractButton:hover {background-color: #ffa010;}"
                                   "QAbstractButton:pressed {background-color: #d07000;}"
                                   "QAbstractButton:disabled {background-color: #303030; color: #808080}");
    emit escPressed(true);
    ui->escLabel->setVisible(false);
}

// void Interface::createBody(float x, float y, float width, float height,
//                            float angle) {
//     model.addObject(x, y, width, height, qDegreesToRadians(angle));
//     sprites.append(
//         QPair<QPixmap, Ingredient>(
//             QPixmap(),
//             Ingredient(
//                 QPoint((x - width / 2) * SCALE, (y - height / 2) * SCALE),
//                 QSize(width * SCALE, height * SCALE),
//                 angle,
//                 QPixmap())
//             )
//         );
//     QLabel* tempLabel = new QLabel(ui->centralwidget);
//     // tempLabel->setStyleSheet("QLabel { background-color: rgba(255, 0, 0, 10) }");
//     tempLabel->setAlignment(Qt::AlignCenter);
//     bodyDisplays.append(tempLabel);

//     // connect(tempLabel, &PixelEditorLabel::pixelEditorLabelClicked,
//     //         this, &Interface::pixelEditorLabelClicked);

// }

void Interface::createLabels(QVector<Ingredient> ingredients) {
    for (int i = 0; i < ingredients.size(); i++) {
        QLabel* tempLabel = new QLabel(ui->centralwidget);
        tempLabel->setStyleSheet("QLabel { background-color: rgba(255, 0, 0, 10) }");
        tempLabel->setAlignment(Qt::AlignCenter);
        tempLabel->setVisible(true);
        bodyDisplays.append(tempLabel);
        qDebug() << "Label created";
    }
}

void Interface::updateObject(int index, Ingredient ingredient) {
    // BELOW IS TEMP FOR A TEST, BUT MAY WORK AS A BASE
    // Ok, so right now the box doesn't have the acurate size because it is
    // impossibly complicated to get a bodies size for some reason
    // So, we are making a different class to store a sprite's info right?
    // Put the size in there
    double width = ingredient.getDimensions().width() * SCALE;
    double height = ingredient.getDimensions().height() * SCALE;
    double x = ingredient.getPosition().x() * SCALE;
    double y = ingredient.getPosition().y() * SCALE;

    double diameter = std::sqrt(std::pow(width, 2) + std::pow(height, 2));
    double angle = ingredient.getOrientation();

    bodyDisplays[bodyDisplays.size() - index - 1]->setGeometry(
        QRect(x - diameter / 2, y - diameter / 2, diameter, diameter));

    // Load the texture, scale it, then transform it using a QTransform that is
    // set to the same angle as the source b2Body.
    QPixmap texture = QPixmap(
        ":/ingredients/assets/images/sprites/EmptyBowl.png");
    texture = texture.scaled(width, height, Qt::KeepAspectRatio);
    QTransform transform;
    transform.rotate(angle);
    texture = texture.transformed(transform);
    // Apply the tranform.
    bodyDisplays[bodyDisplays.size() - index - 1]->setPixmap(texture);

    // qDebug() << "Object at index" << index
    //          << "updated at" << x << y
    //          << "with dimensions" << width << height
    //          << "and angle" << angle
    //          << bodyDisplays[bodyDisplays.size() - index - 1]->isVisible();
}

void Interface::createGround(b2Vec2 loc, int width, int height) {
    // Temp until we store other objects
    ui->ground->setGeometry(loc.x * SCALE, loc.y * SCALE - height * SCALE/2,
                            width * SCALE, height * SCALE);
    ui->ground->setStyleSheet(
        "QLabel { background-color : brown; color : black; }");
}

void Interface::startLevel() {
    emit escPressed(false);
    isStartMenu = false;
    ui->escLabel->setVisible(true);
    ui->escLabel->raise();
    ui->startWidget->setEnabled(false);
    ui->startWidget->setVisible(false);

    emit createWorld();
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

    isStartMenu = true;
    ui->startWidget->setEnabled(true);
    ui->startWidget->setVisible(true);
    ui->startWidget->raise();
    ui->startWidget->raise();
    ui->escLabel->setVisible(false);
}

void Interface::mouseMoveEvent(QMouseEvent* event) {
    if(mouseIsDown)
        emit mouseMoved(QPointF(event->pos().x() * 1.0 / SCALE,
                                event->pos().y() * 1.0 / SCALE));
}

void Interface::mousePressEvent(QMouseEvent* event) {
    QPoint mouseLoc = event->pos();
    qDebug() << mouseLoc.x() << " | " << mouseLoc.y();

    mouseIsDown = true;

    emit mousePressed(QPointF(event->pos().x() * 1.0 / SCALE,
                              event->pos().y() * 1.0 / SCALE));

}

void Interface::mouseReleaseEvent(QMouseEvent* event) {
    mouseIsDown = false;
    selectedObjectIndex = -1;
    emit mouseReleased();
}

void Interface::keyPressEvent(QKeyEvent *event){
    if(isStartMenu)
        return;
    if(event->key() == Qt::Key_Escape) {
        isGamePaused = !isGamePaused;
        emit escPressed(isGamePaused);
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

Interface::~Interface() {
    delete ui;
}
