#ifndef MODEL_H
#define MODEL_H

#include <Box2D/Box2D.h>
#include <QTimer>
#include <QVector>

#include "ingredient.h"

class Model : public QObject {
    Q_OBJECT
private:
    ///
    /// \brief timer - Used to execute the physics loop
    ///
    QTimer timer;
    b2World world;
    b2Body* selected;
    QPoint recentMouseLoc;
    float oldVX;
    float oldVY;

    void modelUpdated(int index, Ingredient rectangle);

public:
    ///
    /// \brief Model - Create a new model to store a new world and new objects
    ///
    Model();
    ~Model();

    ///
    /// \brief addObject Add a dynamic body to the world.
    /// \param x the x position, 0 is top and positive is right
    /// \param y the y position, 0 is top and positive is down
    /// \param width the width
    /// \param height the height
    /// \param angle the angle in radians
    ///
    void addObject(float x, float y, float width, float height, float angle);
    void removeObject(qsizetype index);

public slots:
    void updateWorld();
    void objectClicked(int index, float x, float y);
    void objectReleased();
    void pauseGame(bool pausedState);

signals:
    void objectUpdated(int index, const b2Body* body);
    void makeGroundInView(b2Vec2 loc, int width, int height);

};

#endif // MODEL_H
