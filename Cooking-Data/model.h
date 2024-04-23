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
    /// \brief combinations The valid combinations where the key is a QPair
    ///                     that holds the two source IngredientTypes and the
    ///                     value is a QPair that holds a QVector of all the
    ///                     output IngredientTypes and an int for the delay in
    ///                     milliseconds between initial contact and combining.
    ///                     If the second IngredientType is None, it means it
    ///                     is a transformation involving only one input and
    ///                     usually a delay. If the output contains nothing,
    ///                     nothing will be spawned.
    ///
    QHash<QPair<IngredientType, IngredientType>,
          QPair<QVector<IngredientType>, int>> combinations;

    ///
    /// \brief combinationTimers The timers for combinations where the key is
    ///                          a QPair that holds the IDs of the two
    ///                          Ingredients and the value is a long long that
    ///                          holds the system epoch time in ms when the
    ///                          combination should complete if they are still
    ///                          in contact.
    ///
    QMap<QPair<int, int>, long long> combinationTimers;

    void addIngredient(IngredientType type, QPointF position, double angle);
    void addIngredient(IngredientType type, QPointF position);

    Ingredient* createIngredient(IngredientType type, QPointF position, double angle);

    ///
    /// \brief addIngredientToWorld Add a dynamic body to the world.
    /// \param ingredient the ingredient
    /// \return pointer to the b2Body
    ///
    b2Body* addIngredientToWorld(const Ingredient& ingredient);

    ///
    /// \brief removeIngredient Remove the Ingredient with the ID. Returns true
    ///                         if successful, false otherwise.
    /// \param ingredientID the ID of the Ingredient to remove
    /// \return is removal successful
    ///
    bool removeIngredient(int ingredientID);

    ///
    /// \brief tryCombine Tries to combine the two input Ingredients, if
    ///                   possible.
    ///
    /// This function deletes the two inputs and replaces them with their
    /// associated outputs if the Ingredients can be combined. When
    /// combined, the first Ingredient will always snap to the position of i1
    /// and every subsequent one will snap to the position of i2.
    ///
    /// This function DOES NOT check if the reverse order can be combined!
    ///
    /// If the ID of the second Ingredient is -1, it tries to perform a
    /// transformation instead, where there is only one input Ingredient and all
    /// outputs snap to the position of i1.
    ///
    /// If the first IngredientType matches the IngredientType of i1, i1 is not
    /// deleted and IngredientType is not spawned.
    ///
    /// If the second IngredientType matches the IngredientType of i2, i2 is not
    /// deleted and IngredientType is not spawned.
    ///
    /// \param i1 the ID of the first Ingredient to combine
    /// \param i2 the ID of the second Ingredient to combine, if it is -1, the
    ///           function will perform a transformation instead (combination
    ///           with only one input)
    /// \return true if the Ingredients successfully combined, false otherwise
    ///
    bool tryCombine(int i1, int i2);

    ///
    /// \brief combine Combines the two input Ingredients.
    ///
    /// This function deletes the two inputs and replaces with their associated
    /// outputs. When combined, the first Ingredient will always snap to the
    /// position of i1 and every subsequent one will snap to the position of
    /// i2.
    ///
    /// If the ID of the second Ingredient is -1, it tries to perform a
    /// transformation instead, where there is only one input Ingredient and
    /// all outputs snap to the position of i1.
    ///
    /// If the first output IngredientType matches the IngredientType of i1, i1
    /// is not deleted and the output IngredientType is not spawned.
    ///
    /// If the second output IngredientType matches the IngredientType of i2, i2
    /// is not deleted and the output IngredientType is not spawned.
    ///
    /// \param i1 the ID of the first Ingredient to combine
    /// \param i2 the ID of the second Ingredient to combine, if it is -1, the
    ///           function will perform a transformation instead (combination
    ///           with only one input)
    /// \return true if the existing Ingredients were successfully deleted
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
    void winConditionMet();
};

#endif // MODEL_H
