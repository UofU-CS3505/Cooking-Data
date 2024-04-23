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

    ///
    /// \brief ingredients A map of all ingredients in the scene from their ID
    ///                    to the Ingredient.
    ///
    QMap<int, Ingredient*> ingredients;

    ///
    /// \brief combinations The valid combinations where the first QPair holds
    ///                     the two source IngredientTypes, the second QPair
    ///                     holds a QVector of all the output IngredientTypes
    ///                     and an int for the delay in milliseconds between
    ///                     initial contact and combining.
    ///
    QHash<QPair<IngredientType, IngredientType>,
          QPair<QVector<IngredientType>, int>> combinations;

    ///
    /// \brief combinationTimers The timers for combinations where the first
    ///                          QPair holds the IDs of the two Ingredients and
    ///                          the second QPair holds the start time of the
    ///                          combination and the length of the delay.
    ///
    QMap<QPair<int, int>, QPair<long, int>> combinationTimers;

    float oldVX;
    float oldVY;

    void addIngredient(IngredientType type, QPointF position);

    Ingredient* createIngredient(IngredientType type, QPointF position, double angle);

    ///
    /// \brief addIngredientToWorld Add a dynamic body to the world.
    /// \param ingredient the ingredient
    /// \return pointer to the b2Body
    ///
    b2Body* addIngredientToWorld(const Ingredient& ingredient);

    ///
    /// \brief removeIngredient Remove the ingredient with the ID. Returns true of successful,
    ///                         false otherwise.
    /// \param ingredientID the ID of the ingredient to remove
    /// \return is removal successful
    ///
    bool removeIngredient(int ingredientID);

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
    /// \param i1 the ID of the first ingredient to combine
    /// \param i2 the ID of the second ingredient to combine
    /// \return true if the ingredients successfully combined, false otherwise
    ///
    bool combine(int i1, int i2);

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
    void ingredientUpdated(const Ingredient& ingredient);
    void frameEnded();
    void makeGroundInView(b2Vec2 loc, int width, int height);

};

#endif // MODEL_H
