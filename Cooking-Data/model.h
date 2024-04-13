#ifndef MODEL_H
#define MODEL_H

#include <Box2D/Box2D.h>
#include <QTimer>
#include <QVector>

#include "rectangle.h"

class Model : public QObject
{
    Q_OBJECT
public:

    ///
    /// \brief Model - Create a new model to store a new world and new objects
    ///
    Model();
    ~Model();

    void addObject(float x, float y, float width, float height);
    void removeObject(qsizetype index);

public slots:
    void updateWorld();

signals:
    void objectUpdated(int index, const b2Body* body);
    void makeGroundInView(b2Vec2 loc, int width, int height);

private:
    ///
    /// \brief timer - Used to execute the physics loop
    ///
    QTimer timer;
    b2World world;

    void modelUpdated(int index, Rectangle rectangle);
};

#endif // MODEL_H
