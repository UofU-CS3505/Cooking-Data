#ifndef MODEL_H
#define MODEL_H

#include <Box2D/Box2D.h>
#include <QTimer>
#include <QVector>

#include "ingredient.h"

class Model : public QObject {
    Q_OBJECT
private:
    const double FRAME_TIME = 1000.0 / 60;

    ///
    /// \brief timer - Used to execute the physics loop
    ///
    QTimer timer;
    b2World world;
    b2Body* selected;
    QPointF recentMouseLoc;
    IngredientType winCondition;

    QMap<int, Ingredient*> ingredients;
    QHash<QPair<IngredientType, IngredientType>, QVector<IngredientType>> combinations;
    QHash<Ingredient*, b2Body*> ingredientToBody;

    float oldVX;
    float oldVY;

    void addIngredient(IngredientType type, QPointF position);

    ///
    /// \brief addIngredientToWorld Add a dynamic body to the world.
    /// \param ingredient the ingredient position, 0 is top and positive is right
    /// \param y the y position, 0 is top and positive is down
    /// \param width the width
    /// \param height the height
    /// \param angle the angle in radians
    /// \return pointer to the b2Body
    ///
    b2Body* addIngredientToWorld(const Ingredient& ingredient);
    void removeBox2DObject(qsizetype index);

    Ingredient* createIngredient(IngredientType type, QPointF position, double angle);

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
    ///                combined. When combined, the first ingredient will always
    ///                snap to the position of i1 and every subsequent one will
    ///                snap to the position of i2.
    /// \param i1 the first ingredient to combine
    /// \param i2 the second ingredient to combine
    /// \return true if the ingredients successfully combined, false otherwise
    ///
    bool combine(const Ingredient& i1, const Ingredient& i2);

public slots:
    void createWorld(int level);
    void updateWorld();
    void deleteWorld();
    void pointPressed(QPointF position);
    void pointMoved(QPointF position);
    void pointReleased();
    void pauseGame(bool pausedState);

signals:
    void frameBegan();
    void ingredientUpdated(Ingredient& ingredient);
    void frameEnded();
    void makeGroundInView(b2Vec2 loc, int width, int height);

};

#endif // MODEL_H
