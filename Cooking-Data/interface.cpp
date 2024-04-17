#include "interface.h"
#include "ui_interface.h"
#include "model.h"
#include <QLayout>

Interface::Interface(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Interface),
    model(Model()) {
    ui->setupUi(this);

    // Adding some text boxes
    createBody(6.0f, 0.0f, 1.0f, 1.0f, 0);
    createBody(1.5f, 1.0f, 2.0f, 2.0f, 0);
    createBody(12.0f, -2.0f, 3.0f, 3.0f, 0);
    createBody(25.0f, 0.0f, 10.0f, 10.0f, 0);

    // Connect game updates to the view
    connect(&model, &Model::makeGroundInView,
            this, &Interface::createGround);
    connect(&model, &Model::objectUpdated,
            this, &Interface::updateObject);

    // Connect mouse updates to model
    connect(this, &Interface::draggableClicked,
            &model, &Model::objectClicked);
}

void Interface::createBody(float x, float y, float halfWidth, float halfHeight,
                           double angle) {
    model.addObject(x, y, halfWidth, halfHeight);
    sprites.append(
        QPair<QPixmap, Rectangle>(
            QPixmap(),
            Rectangle(
                QPoint((x - halfWidth) * SCALE, (y - halfHeight) * SCALE),
                QSize(halfWidth * SCALE * 2, halfHeight * SCALE * 2),
                angle)
            )
        );
    QLabel* tempLabel = new QLabel(ui->centralwidget);
    tempLabel->setStyleSheet("QLabel { background-color : red; color : blue;}");
    bodyDisplays.append(tempLabel);

    // connect(tempLabel, &PixelEditorLabel::pixelEditorLabelClicked,
    //         this, &Interface::pixelEditorLabelClicked);

}

void Interface::updateObject(int index, const b2Body* source) {
    //BELOW IS TEMP FOR A TEST, BUT MAY WORK AS A BASE
    // Ok, so right now the box doesn't have the acurate size because it is impossibly
    // complicated to get a bodies size for some reason
    // So, we are making a different class to store a sprite's info right?
    // Put the size in there
    QSize boxSize = sprites[sprites.size() - index - 1].second.getDimensions();
    bodyDisplays[sprites.size() - index - 1]->setGeometry(
        QRect(source->GetPosition().x * SCALE - boxSize.width()/2,
        source->GetPosition().y * SCALE - boxSize.height()/2,
        boxSize.width(), boxSize.height()));
    sprites[sprites.size() - index - 1].second.setPosition(
        QPoint(source->GetPosition().x * SCALE - boxSize.width()/2,
        source->GetPosition().y * SCALE - boxSize.height()/2));
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
}

Interface::~Interface() {
    delete ui;
}
