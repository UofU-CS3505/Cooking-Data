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
    createBody(1.5f, 1.0f, 2.0f, 1.0f, 0);
    createBody(12.0f, -2.0f, 1.0f, 3.0f, 0);
    createBody(25.0f, 0.0f, 10.0f, 10.0f, 0);

    // Connect game updates to the view
    connect(&model, &Model::makeGroundInView, this, &Interface::createGround);
    connect(&model, &Model::objectUpdated, this, &Interface::updateObject);
}

void Interface::createBody(float x, float y, float halfWidth, float halfHeight, double angle)
{
    model.addObject(x, y, halfWidth, halfHeight);
    sprites.append(QPair<QPixmap, Rectangle>(QPixmap(), Rectangle(QPoint((x - halfWidth) * SCALE, (y - halfHeight) * SCALE),
                                                        QSize(halfWidth * SCALE * 2, halfHeight * SCALE * 2),
                                                        angle)));
    QLabel* tempLabel = new QLabel(ui->centralwidget);
    tempLabel->setStyleSheet("QLabel { background-color : red; color : blue; }");
    bodyDisplays.append(tempLabel);
}

void Interface::updateObject(int index, const b2Body* source)
{
    //BELOW IS TEMP FOR A TEST, BUT MAY WORK AS A BASE
    // Ok, so right now the box doesn't have the acurate size because it is impossibly
    // complicated to get a bodies size for some reason
    // So, we are making a different class to store a sprite's info right?
    // Put the size in there
    QSize boxSize = sprites[sprites.size() - index - 1].second.getDimensions();
    bodyDisplays[sprites.size() - index - 1]->setGeometry(QRect(source->GetPosition().x * SCALE - boxSize.width()/2,
                                           source->GetPosition().y * SCALE - boxSize.height()/2,
                                           boxSize.width(), boxSize.height()));
}

void Interface::createGround(b2Vec2 loc, int width, int height)
{
    // Temp until we store other objects
    qDebug() << loc.x << " "  << loc.y << " " << width << " " << height;
    ui->ground->setGeometry(loc.x * SCALE, loc.y * SCALE - height * SCALE/2, width * SCALE, height * SCALE);
    ui->ground->setStyleSheet("QLabel { background-color : brown; color : black; }");
}

Interface::~Interface() {
    delete ui;
}
