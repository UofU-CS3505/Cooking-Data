#include "interface.h"
#include "ui_interface.h"
#include "model.h"

Interface::Interface(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Interface),
    model(Model()) {
    ui->setupUi(this);

    // Just a test label
    ui->test->setStyleSheet("QLabel { background-color : red; color : blue; }");
    ui->test_2->setStyleSheet("QLabel { background-color : red; color : blue; }");

    model.addObject(6.0f,0.0f, 1.0f, 1.0f);
    model.addObject(1.0f,0.0f, 1.0f, 1.0f);

    boxes.append(ui->test);
    boxes.append(ui->test_2);

    // Connect game updates to the view
    connect(&model, &Model::makeGroundInView, this, &Interface::createGround);
    connect(&model, &Model::objectUpdated, this, &Interface::updateObject);
}

void Interface::updateObject(int index, const b2Body* source)
{
    //BELOW IS TEMP FOR A TEST, BUT MAY WORK AS A BASE
    // Ok, so right now the box doesn't have the acurate size because it is impossibly
    // complicated to get a bodies size for some reason
    // So, we are making a different class to store a sprite's info right?
    // Put the size in there
    boxes[index]->setGeometry(QRect(source->GetPosition().x * 10 - 15, source->GetPosition().y * 10 - 15,
                                    30, 30));
}

void Interface::createGround(b2Vec2 loc, int width, int height)
{
    // Temp until we store other objects
    qDebug() << loc.x << " "  << loc.y << " " << width << " " << height;
    ui->ground->setGeometry(loc.x * 10, loc.y*10 - height*10/2, width * 10, height * 10);
    ui->ground->setStyleSheet("QLabel { background-color : brown; color : black; }");
}

Interface::~Interface() {
    delete ui;
}
