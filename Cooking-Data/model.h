#ifndef MODEL_H
#define MODEL_H

#include <Box2D/Box2D.h>
#include <QTimer>
#include <QVector>
#include <QMap>
#include <QPair>

#include "ingredient.h"

class Model : public QObject {
    Q_OBJECT
private:
    const double FRAME_TIME = 1000.0 / 60;

    ///
    /// \brief timer Used to execute the physics loop
    ///
    QTimer timer;
    ///
    /// \brief world Used to hold all Box2D elements
    ///
    b2World world;
    ///
    /// \brief selected The selected body by the mouse
    ///
    b2Body* selected;
    ///
    /// \brief recentMouseLoc The most recent location of the mouse
    ///
    QPointF recentMouseLoc;
    ///
    /// \brief winCondition This ingredient must be made to win the level
    ///
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
    /// \brief tools The Ingredients that must be selected before they can be
    ///              combined or transformed.
    ///
    QVector<IngredientType> tools;

    ///
    /// \brief combinationTimers The timers for combinations where the key is
    ///                          a QPair that holds the IDs of the two
    ///                          Ingredients and the value is a long long that
    ///                          holds the system epoch time in ms when the
    ///                          combination should complete if they are still
    ///                          in contact.
    ///
    QMap<QPair<int, int>, long long> combinationTimers;

    ///
    /// \brief addIngredient Create an Ingredient object, insert it into the
    ///                      map of all Ingredients, and create a b2Body that
    ///                      corresponds to the Ingredient.
    /// \param type the IngredientType to create
    /// \param position the position of the Ingredient
    /// \param angle the angle of the Ingredient in degrees
    ///
    void addIngredient(IngredientType type, QPointF position, double angle);

    ///
    /// \brief addIngredient Create an Ingredient object, insert it into the
    ///                      map of all Ingredients, and create a b2Body that
    ///                      corresponds to the Ingredient.
    /// \param type the IngredientType to create
    /// \param position the position of the Ingredient
    ///
    void addIngredient(IngredientType type, QPointF position);

    ///
    /// \brief createIngredient Create an Ingredient object and returns its
    ///                         pointer. This is a factory function and does
    ///                         not put the Ingredient inside any data
    ///                         structure.
    /// \param type the IngredientType to create
    /// \param position the position of the Ingredient
    /// \param angle the angle of the Ingredient in degrees
    /// \return pointer to the created Ingredient
    ///
    Ingredient* createIngredient(IngredientType type, QPointF position, double angle);

    ///
    /// \brief addIngredientToWorld Add a b2body that corresponds to the
    ///                             provided Ingredient to the world.
    /// \param ingredient the Ingredient
    /// \return pointer to the created b2Body
    ///
    b2Body* addIngredientToWorld(const Ingredient& ingredient);

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

    ///
    /// \brief removeIngredient Remove the Ingredient with the ID. Returns true
    ///                         if successful, false otherwise.
    /// \param ingredientID the ID of the Ingredient to remove
    /// \return is removal successful
    ///
    bool removeIngredient(int ingredientID);

    ///
    /// \brief areIngredientsNotToolsOrSelectedTool Return true if neither i1
    ///                                             nor i2 are tools or if one
    ///                                             or more of i1 or i2 are
    ///                                             tools, the selected
    ///                                             Ingredient is either i1 or
    ///                                             i2 and is a tool.
    /// \param i1 the ID of the first Ingredient
    /// \param i2 the ID of the second Ingredient
    /// \return see brief
    ///
    bool areIngredientsNotToolsOrSelectedTool(int i1, int i2);

public:
    ///
    /// \brief Model Create a new model to store a new world and new objects
    ///
    Model();
    ~Model();

private slots:
    ///
    /// \brief updateWorld Update the bodies and check for any combinations
    ///
    void updateWorld();

public slots:
    ///
    /// \brief createWorld Create the level based on the level input
    /// \param level The level to load: 1-3
    ///
    void createWorld(int level);
    ///
    /// \brief deleteWorld Delete everything in the world
    ///
    void deleteWorld();
    ///
    /// \brief pointPressed Calculate if the given point is in any body,
    ///                     make that the selected object.
    /// \param position The position of the mouse
    ///
    void pointPressed(QPointF position);
    ///
    /// \brief pointMoved Update the saved location of the mouse
    /// \param position The position of the mouse
    ///
    void pointMoved(QPointF position);
    ///
    /// \brief pointReleased Update the selected object to be nothing
    ///
    void pointReleased();
    ///
    /// \brief pauseGame Pause the game, bodies are not updated
    /// \param pausedState A bool to either pause or unpause the game
    ///
    void pauseGame(bool pausedState);

signals:
    ///
    /// \brief frameBegan Send everytime the world is updated
    ///
    void frameBegan();
    ///
    /// \brief ingredientUpdated Send when an ingredient is updated
    /// \param ingredient
    ///
    void ingredientUpdated(const Ingredient& ingredient);
    ///
    /// \brief frameEnded Notify the view when the update to the world is done
    ///
    void frameEnded();
    ///
    /// \brief winConditionMet Notify the view that the level should be complete
    ///
    void winConditionMet();
    ///
    /// \brief comboCompleted Let the view know that a combo was completed
    ///
    void comboCompleted();
};

#endif // MODEL_H
