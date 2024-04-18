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

    QHash<QPair<IngredientType, IngredientType>, IngredientType> combinations;
    QSet<Ingredient> activeIngredients;

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

    ///
    /// \brief combine Combines the two input ingredients, if possible. This
    ///                method deletes the two inputs and replaces them with
    ///                their associated outputs if the ingredients can be
    ///                combined. When combined, the new ingredient will always
    ///                snap to the position of i1.
    /// \param i1 the first ingredient to combine
    /// \param i2 the second ingredient to combine
    /// \return true if the ingredients successfully combined, false otherwise
    ///
    bool combine(const Ingredient& i1, const Ingredient& i2);

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
