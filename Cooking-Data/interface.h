#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMainWindow>
#include <QLabel>

#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Interface;
}
QT_END_NAMESPACE

class Interface : public QMainWindow
{
    Q_OBJECT

public:
    Interface(QWidget *parent = nullptr);
    ~Interface();

public slots:
    void updateObject(int index, const b2Body* source);
    void createGround(b2Vec2 loc, int width, int height);

private:
    Ui::Interface *ui;
    Model model;
    QVector<QLabel*> boxes;
};
#endif // INTERFACE_H
