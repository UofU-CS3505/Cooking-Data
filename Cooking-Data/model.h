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
    /// \brief timer Used to execute the physics loop
    ///
    QTimer timer;
    b2World world;
    b2Body* selected;
    QPointF recentMouseLoc;
    IngredientType winCondition;

    ///
    /// \brief ingredients A map of all Ingredients in the scene from their ID
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
    /// \brief combinationTimers The timers for combinations where the QPair
    ///                          holds the IDs of the two Ingredients and the
    ///                          long holds the system epoch time in ms when the
    ///                          combination should complete if they are still
    ///                          in contact.
    ///
    QMap<QPair<int, int>, long long> combinationTimers;

    void addIngredient(IngredientType type, QPointF position);

    Ingredient* createIngredient(IngredientType type, QPointF position, double angle);

    ///
    /// \brief addIngredientToWorld Add a dynamic body to the world.
    /// \param ingredient the ingredient
    /// \return pointer to the b2Body
    ///
    b2Body* addIngredientToWorld(const Ingredient& ingredient);

    ///
    /// \brief removeIngredient Remove the Ingredient with the ID. Returns true of successful,
    ///                         false otherwise.
    /// \param ingredientID the ID of the Ingredient to remove
    /// \return is removal successful
    ///
    bool removeIngredient(int ingredientID);

    ///
    /// \brief tryCombine Combines the two input Ingredients, if possible. This
    ///                   function deletes the two inputs and replaces them with
    ///                   their associated outputs if the Ingredients can be
    ///                   combined. When combined, the first Ingredient will
    ///                   always snap to the position of i1 and every subsequent
    ///                   one will snap to the position of i2. This function
    ///                   DOES NOT check if the reverse order can be combined!
    /// \param i1 the ID of the first Ingredient to combine
    /// \param i2 the ID of the second Ingredient to combine
    /// \return true if the Ingredients successfully combined, false otherwise
    ///
    bool tryCombine(int i1, int i2);

    ///
    /// \brief combine Combines the two input Ingredients. This function deletes
    ///                the two inputs and replaces with their associated
    ///                outputs. When combined, the first Ingredient will always
    ///                snap to the position of i1 and every subsequent one will
    ///                will snap to the position of i2.
    /// \param i1 the ID of the first Ingredient to combine
    /// \param i2 the ID of the second Ingredient to combine
    /// \return true if the existing Ingredients were successfully deleted.
    ///
    bool combine(int i1, int i2);

public:
    ///
    /// \brief Model Create a new model to store a new world and new objects
    ///
    Model();
    ~Model();

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
