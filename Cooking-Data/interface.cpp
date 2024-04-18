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

    // Adding some text boxes
    createBody(6.0f, 0.0f, 1.5f, 1.0f, 0);
    createBody(1.5f, 0.0f, 3.0f, 2.0f, 0);
    createBody(1.5f, 2.0f, 3.0f, 2.0f, 90);
    createBody(1.5f, 4.0f, 3.0f, 2.0f, 180);
    createBody(1.5f, 6.0f, 3.0f, 2.0f, 270);
    createBody(12.0f, -2.0f, 4.5f, 3.0f, 0);
    createBody(25.0f, 0.0f, 9.0f, 6.0f, 0);

    // Connect game updates to the view
    connect(&model, &Model::makeGroundInView,
            this, &Interface::createGround);
    connect(&model, &Model::objectUpdated,
            this, &Interface::updateObject);

    // Connect mouse updates to model
    connect(this, &Interface::draggableClicked,
            &model, &Model::objectClicked);
    connect(this, &Interface::draggableReleased,
            &model, &Model::objectReleased);
}

void Interface::createBody(float x, float y, float width, float height,
                           float angle) {
    model.addObject(x, y, width, height, qDegreesToRadians(angle));
    sprites.append(
        QPair<QPixmap, Ingredient>(
            QPixmap(),
            Ingredient(
                QPoint((x - width / 2) * SCALE, (y - height / 2) * SCALE),
                QSize(width * SCALE, height * SCALE),
                angle,
                QPixmap())
            )
        );
    QLabel* tempLabel = new QLabel(ui->centralwidget);
    // tempLabel->setStyleSheet("QLabel { background-color: rgba(255, 0, 0, 10) }");
    tempLabel->setAlignment(Qt::AlignCenter);
    bodyDisplays.append(tempLabel);

    // connect(tempLabel, &PixelEditorLabel::pixelEditorLabelClicked,
    //         this, &Interface::pixelEditorLabelClicked);

}

void Interface::updateObject(int index, const b2Body* source) {
    // BELOW IS TEMP FOR A TEST, BUT MAY WORK AS A BASE
    // Ok, so right now the box doesn't have the acurate size because it is
    // impossibly complicated to get a bodies size for some reason
    // So, we are making a different class to store a sprite's info right?
    // Put the size in there
    QSize boxSize = sprites[sprites.size() - index - 1].second.getDimensions();
    double width = boxSize.width();
    double height = boxSize.height();
    double x = source->GetPosition().x;
    double y = source->GetPosition().y;

    double diameter = std::sqrt(std::pow(width, 2) + std::pow(height, 2));
    double angle = qRadiansToDegrees(source->GetAngle());

    bodyDisplays[sprites.size() - index - 1]->setGeometry(
        QRect(x * SCALE - width / 2 - (diameter - width) / 2,
              y * SCALE - height / 2 - (diameter - height) / 2,
              diameter, diameter));

    // Load the texture, scale it, then transform it using a QTransform that is
    // set to the same angle as the source b2Body.
    QPixmap texture = QPixmap(
        ":/ingredients/assets/images/sprites/EmptyBowl.png");
    texture = texture.scaled(width, height, Qt::KeepAspectRatio);
    QTransform transform;
    transform.rotate(angle);
    texture = texture.transformed(transform);
    // Apply the tranform.
    bodyDisplays[sprites.size() - index - 1]->setPixmap(texture);

    sprites[sprites.size() - index - 1].second.setPosition(
        QPoint(x * SCALE - width / 2,
               y * SCALE - height / 2));
}

void Interface::createGround(b2Vec2 loc, int width, int height) {
    // Temp until we store other objects
    ui->ground->setGeometry(loc.x * SCALE, loc.y * SCALE - height * SCALE/2,
                            width * SCALE, height * SCALE);
    ui->ground->setStyleSheet(
        "QLabel { background-color : brown; color : black; }");
}

void Interface::mousePressEvent(QMouseEvent* event) {
    QPoint mouseLoc = event->pos();
    qDebug() << mouseLoc.x() << " | " << mouseLoc.y();

    // Iterate every object, emit if mouse colliding with this
    for (int i = 0; i < sprites.size(); i++){
        auto item = sprites[i];
        int x1 = item.second.getPosition().x();
        int y1 = item.second.getPosition().y();
        int x2 = item.second.getPosition().x()
                 + item.second.getDimensions().width();
        int y2 = item.second.getPosition().y()
                 + item.second.getDimensions().height();

        qDebug() << x1 << " x1 | x2 " << x2;
        qDebug() << y1 << " y1 | y2 " << y2;
        if(x1 <= mouseLoc.x() && x2 >= mouseLoc.x()
           && y1 <= mouseLoc.y() && y2 >= mouseLoc.y()) {
            // Mouse is selecting this One!
            qDebug() << "Emit";
            emit draggableClicked(
                sprites.size() - i - 1,
                mouseLoc.x() / SCALE,
                mouseLoc.y() / SCALE);
            selectedObjectIndex = sprites.size() - i - 1;
            mouseIsDown = true;
            return;
        }
    }
}

void Interface::mouseMoveEvent(QMouseEvent* event) {
    if(mouseIsDown)
        emit draggableClicked(selectedObjectIndex,
                              event->pos().x() / SCALE,
                              event->pos().y() / SCALE);
}

void Interface::mouseReleaseEvent(QMouseEvent* event) {
    mouseIsDown = false;
    selectedObjectIndex = -1;
    emit draggableReleased();
}

Interface::~Interface() {
    delete ui;
}
