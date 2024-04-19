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
    QPointF recentMouseLoc;
    QHash<QPair<IngredientType, IngredientType>, IngredientType> combinations;
    QHash<Ingredient, b2Body*> ingredientToBody;
    QSet<Ingredient> activeIngredients;
    float oldVX;
    float oldVY;

    void addIngredient(IngredientType type, QPointF position);

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

    Ingredient createIngredient(IngredientType ingType, QPointF position, double angle);

public:
    ///
    /// \brief Model - Create a new model to store a new world and new objects
    ///
    Model();
    ~Model();

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
    void createWorld();
    void updateWorld();
    void pointPressed(QPointF position);
    void pointMoved(QPointF position);
    void pointReleased();
    void pauseGame(bool pausedState);

signals:
    void worldCreated(QSet<Ingredient> ingredients);
    void ingredientUpdated(int index, Ingredient ingredient);
    void makeGroundInView(b2Vec2 loc, int width, int height);

};

#endif // MODEL_H
