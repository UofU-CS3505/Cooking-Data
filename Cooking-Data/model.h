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
    QVector<Ingredient> ingredients;
    QPointF recentMouseLoc;
    float oldVX;
    float oldVY;

    void addIngredient(QPointF position);

    ///
    /// \brief addBox2DObject Add a dynamic body to the world.
    /// \param x the x position, 0 is top and positive is right
    /// \param y the y position, 0 is top and positive is down
    /// \param width the width
    /// \param height the height
    /// \param angle the angle in radians
    ///
    void addBox2DObject(float x, float y, float width, float height, float angle);
    void removeBox2DObject(qsizetype index);
    void modelUpdated(int index, Ingredient rectangle);

public:
    ///
    /// \brief Model - Create a new model to store a new world and new objects
    ///
    Model();
    ~Model();

public slots:
    void createWorld();
    void updateWorld();
    void pointPressed(QPointF position);
    void pointMoved(QPointF position);
    void pointReleased();
    void pauseGame(bool pausedState);

signals:
    void worldCreated(QVector<Ingredient> ingredients);
    void objectUpdated(int index, Ingredient ingredient);
    void makeGroundInView(b2Vec2 loc, int width, int height);

};

#endif // MODEL_H
