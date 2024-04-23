#include <QObject>
#include <QDateTime>
#include <QDebug>

#include "ingredienttype.h"
#include "model.h"

Model::Model()
    : timer(),
    world(b2World(b2Vec2(0.0f, 9.8f))){

    // THIS IS TEST CODE, WE WILL EDIT THIS LATER AS NEEDED
    ////////////////////////////


    // Start the timer.
    connect(&timer, &QTimer::timeout, this, &Model::updateWorld);

    // Let view create the ground

    QTimer::singleShot(50, this, [&] () {
        emit makeGroundInView(b2Vec2(0.0f, 60.0f), 100, 20);
    });
    // makeGroundInView isn't complete, rn we are only storing 1 static object
    // named ground, change this so all walls paint
    QTimer::singleShot(40, this, [&] () {
        emit makeGroundInView(b2Vec2(-10.0f, 30.0f), 10, 100);
    });
    QTimer::singleShot(40, this, [&] () {
        emit makeGroundInView(b2Vec2(90.0f, 30.0f), 10, 100);
    });


    // Define all valid combinations.
    combinations.insert(
        qMakePair(WaterPitcher, EmptyPot),
        qMakePair(QVector<IngredientType> { WaterPitcher, WaterPot }, 0));
    combinations.insert(
        qMakePair(StoveOn, WaterPot),
        qMakePair(QVector<IngredientType> { StoveOn, BoilingWaterPot }, 5000));
    combinations.insert(
        qMakePair(StoveOn, BoilingWaterPot),
        qMakePair(QVector<IngredientType> { StoveOn, EmptyPot  }, 5000));
    combinations.insert(
        qMakePair(StoveOn, EmptyPot),
        qMakePair(QVector<IngredientType> { StoveOn, EmptyPot, Ember }, 2000));
    combinations.insert(
        qMakePair(StoveOn, OatPacket),
        qMakePair(QVector<IngredientType> { StoveOn, OatPacket, Ember }, 1000));
    combinations.insert(
        qMakePair(BoilingWaterPot, None),
        qMakePair(QVector<IngredientType> { WaterPot }, 10000));
    combinations.insert(
        qMakePair(BoilingWaterPot, Ladel),
        qMakePair(QVector<IngredientType> { BoilingWaterPot, WaterLadel }, 0));
    combinations.insert(
        qMakePair(EmptyBowl, OatPacket),
        qMakePair(QVector<IngredientType> { OatsBowl, OatPacket }, 0));
    combinations.insert(
        qMakePair(OatsBowl, WaterLadel),
        qMakePair(QVector<IngredientType> { OatmealBowl, Ladel }, 0));
    combinations.insert(
        qMakePair(OatPacket, Ember),
        qMakePair(QVector<IngredientType> { OatPacket, Ember, Ember }, 1000));
    combinations.insert(
        qMakePair(OatPacket, Fire),
        qMakePair(QVector<IngredientType> { Fire, Fire }, 1900)); // has to be shorter than fire decay
    combinations.insert(
        qMakePair(WaterPitcher, Fire),
        qMakePair(QVector<IngredientType> { WaterPitcher, Ember }, 0));
    combinations.insert(
        qMakePair(WaterPitcher, Ember),
        qMakePair(QVector<IngredientType> { WaterPitcher }, 0));
    combinations.insert(
        qMakePair(WaterPot, Fire),
        qMakePair(QVector<IngredientType> { EmptyPot, Ember }, 0));
    combinations.insert(
        qMakePair(WaterPot, Ember),
        qMakePair(QVector<IngredientType> { EmptyPot }, 0));
    combinations.insert(
        qMakePair(BoilingWaterPot, Fire),
        qMakePair(QVector<IngredientType> { EmptyPot, Ember }, 0));
    combinations.insert(
        qMakePair(BoilingWaterPot, Ember),
        qMakePair(QVector<IngredientType> { EmptyPot }, 0));
    combinations.insert(
        qMakePair(WaterLadel, Fire),
        qMakePair(QVector<IngredientType> { Ladel, Ember }, 0));
    combinations.insert(
        qMakePair(WaterLadel, Ember),
        qMakePair(QVector<IngredientType> { Ladel }, 0));
    combinations.insert(
        qMakePair(Fire, None),
        qMakePair(QVector<IngredientType> { Ember }, 2000));
    combinations.insert(
        qMakePair(Ember, Ember),
        qMakePair(QVector<IngredientType> { Fire }, 0));
    combinations.insert(
        qMakePair(Ember, None),
        qMakePair(QVector<IngredientType> { }, 5000));
}

Model::~Model() {
    // How to delete maps?
    // qDeleteAll(ingredients);
    // qDeleteAll(combinations);
    // qDeleteAll(ingredientToBody);
}

void Model::addIngredient(IngredientType type, QPointF position, double angle) {
    Ingredient* ingredient = createIngredient(type, position, angle);
    qDebug() << "Ingredient made with ID" << ingredient->getID();
    ingredients.insert(ingredient->getID(), ingredient);
    qDebug() << "Inserted ingredient with ID" << ingredients[ingredient->getID()]->getID() << "into map with key" << ingredient->getID();
    addIngredientToWorld(*ingredient);
    qDebug() << "box2d made";
}

void Model::addIngredient(IngredientType type, QPointF position) {
    addIngredient(type, position, 0);
}

Ingredient* Model::createIngredient(IngredientType type, QPointF position, double angle) {
    // Note: the size should be 0.0125x (divide by 80) the dimension of the texture (convert
    // from a scale of 0.5 in. per pixel to meter).

    if (type == BoilingWaterPot)
        return new Ingredient(BoilingWaterPot, QSizeF(0.2, 0.175), 4,
                              QPixmap(":/ingredients/assets/images/sprites/BoilingWaterPot.png"),
                              position, angle);

    if (type == EmptyBowl)
        return new Ingredient(EmptyBowl, QSizeF(0.15, 0.1), 0.5,
                              QPixmap(":/ingredients/assets/images/sprites/EmptyBowl.png"),
                              position, angle);

    if (type == EmptyPot)
        return new Ingredient(EmptyPot, QSizeF(0.2, 0.125), 2,
                              QPixmap(":/ingredients/assets/images/sprites/EmptyPot.png"),
                              position, angle);

    if (type == Ladel)
        return new Ingredient(Ladel, QSizeF(0.1, 0.1625), 0.25,
                              QPixmap(":/ingredients/assets/images/sprites/Ladel.png"),
                              position, angle);

    if (type == OatPacket)
        return new Ingredient(OatPacket, QSizeF(0.175, 0.175), 2,
                              QPixmap(":/ingredients/assets/images/sprites/Oatmeal.png"),
                              position, angle);

    if (type == OatmealBowl)
        return new Ingredient(OatmealBowl, QSizeF(0.15, 0.2125), 3,
                              QPixmap(":/ingredients/assets/images/sprites/OatmealBowl.png"),
                              position, angle);

    if (type == OatsBowl)
        return new Ingredient(OatsBowl, QSizeF(0.15, 0.1), 2,
                              QPixmap(":/ingredients/assets/images/sprites/OatsBowl.png"),
                              position, angle);

    if (type == WaterLadel)
        return new Ingredient(WaterLadel, QSizeF(0.1, 0.1625), 0.5,
                              QPixmap(":/ingredients/assets/images/sprites/WaterLadel.png"),
                              position, angle);

    if (type == WaterPitcher)
        return new Ingredient(WaterPitcher, QSizeF(0.1875, 0.2125), 5,
                              QPixmap(":/ingredients/assets/images/sprites/WaterPitcher.png"),
                              position, angle);

    if (type == WaterPot)
        return new Ingredient(WaterPot, QSizeF(0.2, 0.125), 4,
                              QPixmap(":/ingredients/assets/images/sprites/WaterPot.png"),
                              position, angle);

    if (type == StoveOff)
        return new Ingredient(StoveOff, QSizeF(0.4, 0.2625), 0,
                              QPixmap(":/ingredients/assets/images/sprites/StoveOff.png"),
                              position, angle);

    if (type == StoveOn)
        return new Ingredient(StoveOn, QSizeF(0.4, 0.2625), 0,
                              QPixmap(":/ingredients/assets/images/sprites/StoveOn.png"),
                              position, angle);

    if (type == Fire)
        return new Ingredient(Fire, QSizeF(0.1, 0.15), 0.2,
                              QPixmap(":/ingredients/assets/images/sprites/Fire.png"),
                              position, 0);

    if (type == Ember)
        return new Ingredient(Ember, QSizeF(0.0375, 0.0375), 0.1,
                              QPixmap(":/ingredients/assets/images/sprites/Ember.png"),
                              position, angle);

    return new Ingredient();
}

b2Body* Model::addIngredientToWorld(const Ingredient& ingredient) {
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.position.Set(ingredient.getPosition().x(),
                         ingredient.getPosition().y());
    bodyDef.angle = qDegreesToRadians(ingredient.getAngle());
    b2Body* body = this->world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(ingredient.getDimensions().width() / 2,
                        ingredient.getDimensions().height() / 2);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density.
    fixtureDef.density = ingredient.getWeight()
                         / ingredient.getDimensions().width()
                         / ingredient.getDimensions().height();

    // Override the default friction.
    fixtureDef.friction = 0.8f;
    fixtureDef.restitution = 0.1f;
    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);
    // If the weight is 0, don't set as dynamic body.
    if (ingredient.getWeight() != 0)
        body->SetType(b2_dynamicBody);

    // Set corresponding Ingredient's ID
    // This horrible piece of code converts the int to a void pointer and is from
    // https://stackoverflow.com/questions/30768714/properly-casting-a-void-to-an-integer-in-c
    body->SetUserData(
        reinterpret_cast<void *>(static_cast<intptr_t>(ingredient.getID())));
    return body;
}

bool Model::removeIngredient(int ingredientID) {
    // Ingredient not found!
    if (!ingredients.contains(ingredientID))
        return false;

    // Destroy the body.
    bool isBodyDestroyed = false;
    for (b2Body* body = world.GetBodyList();
         body != nullptr;
         body = body->GetNext()) {
        // This horrible piece of code converts the void pointer from
        // b2Body::GetUserData() to an int
        // https://stackoverflow.com/questions/30768714/properly-casting-a-void-to-an-integer-in-c
        int ingredientIDofBody = static_cast<int>(
            reinterpret_cast<intptr_t>(body->GetUserData()));

        if (ingredientIDofBody != ingredientID)
            continue;

        world.DestroyBody(body);
        isBodyDestroyed = true;
        break;
    }

    // b2Body not found!
    if (!isBodyDestroyed)
        return false;

    // Remove the Ingredient from the list of Ingredients and delete the
    // Ingredient object.
    Ingredient* ingredientToRemove = ingredients[ingredientID];
    ingredients.remove(ingredientID);
    delete ingredientToRemove;

    return true;
}

bool Model::tryCombine(int i1, int i2) {
    // Check if both Ingredients (combination) are in the map of ingredients.
    // If only one Ingredient (transformation), only check if that one is in the
    // map.
    if (!(ingredients.contains(i1) && ingredients.contains(i2))
        && (i2 == -1 && !ingredients.contains(i1)))
        return false;

    QPair<IngredientType, IngredientType> typePair;
    // Check if it is a combination or transformation.
    if (i2 != -1)
        // Is a combination, make a typePair with both IngredientTypes.
        typePair = qMakePair(ingredients[i1]->getIngredientType(),
                    ingredients[i2]->getIngredientType());
    else
        // Is a transformation, make a typePair with the IngredientType of the
        // first Ingredient and None as the second.
        typePair = qMakePair(ingredients[i1]->getIngredientType(),
                    None);

    // Check if the combination is valid.
    if (!combinations.contains(typePair))
        return false;

    // qDebug() << "Trying to combine" << i1 << "and" << i2;

    // Check if the combination requires a waiting time.
    if (combinations[typePair].second == 0)
        // The combination does not require a waiting time.
        return combine(i1, i2);

    // The combination requires a waiting time.
    QPair<int, int> IDPair = qMakePair(i1, i2);

    // Check if the pair is in the map of timers.
    if (!combinationTimers.contains(IDPair)) {
        // The timer is not in the map, add it to the map.
        combinationTimers.insert(IDPair,
                                 QDateTime::currentMSecsSinceEpoch()
                                     + combinations[typePair].second);
        qDebug() << "Added a timer"
                 << (combinationTimers[IDPair] - QDateTime::currentMSecsSinceEpoch())
                        / 1000.0
                 << "seconds in the future for" << i1 << i2;
        return false;
    }

    // qDebug() << "The timer for" << i1 << i2 << "has"
    //          << (combinationTimers[IDPair] - QDateTime::currentMSecsSinceEpoch())
    //                 / 1000.0 << "seconds to go";

    // The timer is already in the map.
    // Check if enough time has elapsed.
    if (QDateTime::currentMSecsSinceEpoch()
        < combinationTimers[IDPair])
        return false;

    // Enough time has passed.
    // Check that it has not been too long.
    if (QDateTime::currentMSecsSinceEpoch()
        > combinationTimers[IDPair] + 1000) {
        // It has been too long, remove the timer.
        combinationTimers.remove(IDPair);
        return false;
    }

    // It has not been too long, combining.
    combinationTimers.remove(IDPair);
    return combine(i1, i2);
}

bool Model::combine(int i1, int i2) {
    QPair<IngredientType, IngredientType> typePair;
    // Check if it is a combination or transformation.
    if (i2 != -1)
        // Is a combination, make a typePair with both IngredientTypes.
        typePair = qMakePair(ingredients[i1]->getIngredientType(),
                             ingredients[i2]->getIngredientType());
    else
        // Is a transformation, make a typePair with the IngredientType of the
        // first input Ingredient and None as the second.
        typePair = qMakePair(ingredients[i1]->getIngredientType(),
                             None);

    // Check if there is any output Ingredient to spawn.
    if (combinations[typePair].first.empty()) {
        // Nothing needs to be spawned.
        if (i2 != -1)
            // Is a combination, remove both input Ingredients.
            return removeIngredient(i1) && removeIngredient(i2);
        else
            // Is a transformation, remove the only input Ingredient.
            return removeIngredient(i1);
    }

    // Decide where to spawn extra output Ingredients and which index extra
    // output Ingredients begin at.
    QPointF spawnPosition;
    double spawnAngle;
    int indexOfRemaingingOutput;
    if (i2 != -1) {
        // Is a combination, set spawn position and angle to that of i2.
        spawnPosition = ingredients[i2]->getPosition();
        spawnAngle = ingredients[i2]->getAngle();
        // Because the first two output Ingredients' spawn will be managed
        // later, set the index of the remaining output to 2.
        indexOfRemaingingOutput = 2;
    } else {
        // Is a transformation, set spawn position and angle to that of i1.
        spawnPosition = ingredients[i1]->getPosition();
        spawnAngle = ingredients[i1]->getAngle();
        // Because only the first output Ingredient's spawn will be managed
        // later, set the index of the remaining output to 1;
        indexOfRemaingingOutput = 1;
    }

    // Spawn the extra output Ingredients.
    for (int i = indexOfRemaingingOutput; i < combinations[typePair].first.size(); i++) {
        addIngredient(combinations[typePair].first[i], spawnPosition, spawnAngle);

        // Check if any extra Ingredients are win condition.
        if (combinations[typePair].first[i] == winCondition)
            qDebug() << "VICTORY ROYALE";
    }

    bool removeSuccessful = true;

    // Check if the type of the first output is different from the type of the
    // first input.
    if (combinations[typePair].first[0] != typePair.first) {
        // The types are different, replace the frst input Ingredient.
        // Spawn the first output Ingredient.
        addIngredient(combinations[typePair].first[0],
                      ingredients[i1]->getPosition(),
                      ingredients[i1]->getAngle());
        // Remove the first input Ingredient.
        removeSuccessful = removeIngredient(i1);

        // Check if the first Ingredient is a win condition.
        if (combinations[typePair].first[0] == winCondition)
            qDebug() << "VICTORY ROYALE";
    }

    // If it is a combination, and there are at least two outputs, and the
    // second output is a different type from the second input, replace the
    // second input with the second output.
    bool replaceSecondIngredient = i2 != -1
                                   && combinations[typePair].first.size() > 1;
    replaceSecondIngredient = // C++ lacks lazy evaluation
        replaceSecondIngredient
        && combinations[typePair].first[1] != typePair.second;

    // Check if second input Ingredient should be replaced or removed.
    if (replaceSecondIngredient) {
        // Spawn the second Ingredient.
        addIngredient(combinations[typePair].first[1],
                      ingredients[i2]->getPosition(),
                      ingredients[i2]->getAngle());
        removeSuccessful = removeSuccessful && removeIngredient(i2);

        // Check if the second Ingredient is a win condition.
        if (combinations[typePair].first[1] == winCondition)
            qDebug() << "VICTORY ROYALE";
    } else if (i2 != -1 && combinations[typePair].first.size() == 1)
        // Is a combination but there is only one output.
        removeSuccessful = removeSuccessful && removeIngredient(i2);


    qDebug() << "Combined and removal" << removeSuccessful;
    return removeSuccessful;
}

void Model::createWorld(int level) {
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 1.36);
    b2Body* groundBody = this->world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(3, 0.1);
    groundBody->CreateFixture(&groundBox, 0);

    // Define static walls
    b2BodyDef leftWallBodyDef;
    leftWallBodyDef.position.Set(-0.11, 0.5);
    b2Body* leftBody = this->world.CreateBody(&leftWallBodyDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(0.1, 1);
    leftBody->CreateFixture(&leftWallBox, 0);

    b2BodyDef rightWallBodyDef;
    rightWallBodyDef.position.Set(2.09, 0.5);
    b2Body* rightBody = this->world.CreateBody(&rightWallBodyDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(0.1, 1);
    rightBody->CreateFixture(&rightWallBox, 0);

    // Add Ingredients.
    if (level == 1) {
        addIngredient(StoveOff, QPointF(1, 1.1));
        addIngredient(EmptyPot, QPointF(0.2, 0));
        addIngredient(OatPacket, QPointF(0.4, 0));
        addIngredient(WaterPitcher, QPointF(0.6, 0));
        addIngredient(Ladel, QPointF(0.8, 0));

        addIngredient(EmptyBowl, QPointF(1.2, 0));
        addIngredient(EmptyBowl, QPointF(1.4, 0));
    } else if (level == 2) {

    } else if (level == 3) {

    } else {
        // Just else makes this a fail safe is something went wrong
        winCondition = OatmealBowl;
    }

    timer.start(FRAME_TIME);
    qDebug() << "World created";
}

void Model::updateWorld() {
    // About 60fps, 10 iterations per collision check
    float32 timeStep = FRAME_TIME / 1000;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    if (selected != nullptr) {
        // SPRING
        // float distanceX = recentMouseLoc.x() - selected->GetPosition().x;
        // float distanceY = recentMouseLoc.y() - selected->GetPosition().y;
        // float velocityX = distanceX * 50;
        // float velocityY = distanceY * 50;
        // b2Vec2 force = b2Vec2(velocityX, velocityY);
        // selected->ApplyForceToCenter(force, true);

        // Gravity
        float distanceX = recentMouseLoc.x() - selected->GetPosition().x;
        float distanceY = recentMouseLoc.y() - selected->GetPosition().y;
        float velocityX = (std::abs(distanceX) / distanceX) // figure out the sign
                          * distanceX * distanceX * 200;
        float velocityY = (std::abs(distanceY) / distanceY) // figure out the sign
                          * distanceY * distanceY * 200;
        b2Vec2 force = b2Vec2(velocityX, velocityY);
        selected->ApplyForceToCenter(force, true);
    }

    world.Step(timeStep, velocityIterations, positionIterations);

    // Check for combinations
    for (b2Contact* collision = world.GetContactList();
         collision != nullptr;
         collision = collision->GetNext()) {
        // Ignore false positives.
        if (!collision->IsTouching())
            continue;

        // This horrible piece of code converts the void pointer from
        // b2Body::GetUserData() to an int
        // https://stackoverflow.com/questions/30768714/properly-casting-a-void-to-an-integer-in-c
        int i1ID = static_cast<int>(reinterpret_cast<intptr_t>(
            collision->GetFixtureA()->GetBody()->GetUserData()));
        int i2ID = static_cast<int>(reinterpret_cast<intptr_t>(
            collision->GetFixtureB()->GetBody()->GetUserData()));

        // If the ingredient was not found in the map, continue the loop.
        if (!(ingredients.contains(i1ID) && ingredients.contains(i2ID)))
            continue;

        // Try both possible order of combinations in case the collision was
        // detected in reverse.
        // Limit combines to once per frame... for reasons.
        if (tryCombine(i1ID, i2ID) || tryCombine(i2ID, i1ID))
            break;
    }

    emit frameBegan();

    // Loop through every body, update their position and angle, send signal
    // any valid Ingredient.
    for (b2Body* body = world.GetBodyList();
         body != nullptr;
         body = body->GetNext()) {

        // This horrible piece of code converts the void pointer from
        // b2Body::GetUserData() to an int
        // https://stackoverflow.com/questions/30768714/properly-casting-a-void-to-an-integer-in-c
        int ingredientID = static_cast<int>(
            reinterpret_cast<intptr_t>(body->GetUserData()));

        // qDebug() << "Checking if Ingredient" << ingredientID << "is in the map";
        // If the ingredient was not found in the map, continue the loop.
        if (!ingredients.contains(ingredientID))
            continue;

        // Try to transform the Ingredient.
        if (tryCombine(ingredientID, -1))
            // Transformed. The Ingredient is gone. Continue the loop to avoid
            // trying to send a nonexistent Ingredient.
            continue;

        Ingredient& ingredient = *ingredients[ingredientID];

        ingredient.setPosition(QPointF(body->GetPosition().x,
                                       body->GetPosition().y));
        ingredient.setAngle(qRadiansToDegrees(body->GetAngle()));

        emit ingredientUpdated(ingredient);
    }

    emit frameEnded();
}

void Model::deleteWorld(){
    for (b2Body* body = world.GetBodyList();
         body != nullptr;
         body = body->GetNext())
        world.DestroyBody(body);
    ingredients.clear();
}

void Model::pointPressed(QPointF position) {
    // Iterate every object to find which one is selected.
    // If nothing is selected, return.
    int selectedIngredientID = -1;

    // Find out which ingredient was selected.
    for (auto [key, value] : ingredients.asKeyValueRange()) {
        double x1 = value->getPosition().x()
                    - value->getDimensions().width() / 2;
        double y1 = value->getPosition().y()
                    - value->getDimensions().height() / 2;
        double x2 = value->getPosition().x()
                    + value->getDimensions().width() / 2;
        double y2 = value->getPosition().y()
                    + value->getDimensions().height() / 2;
        
        // qDebug() << "mouse x " << position.x() << " | mouse y " << position.y();
        // qDebug() << "x1 " << x1 << " | x2 " << x2;
        // qDebug() << "y1 " << y1 << " | y2 " << y2;
        if (x1 <= position.x() && x2 >= position.x()
            && y1 <= position.y() && y2 >= position.y()) {
            // qDebug() << "ID" << key << "; actual ID" << value->getID();
            selectedIngredientID = value->getID();
            // qDebug() << "Selected Ingredient ID:" << selectedIngredientID;
            break;
        }
    }

    // If the index is still -1, it means nothing was selected.
    if (selectedIngredientID == -1) {
        selected = nullptr;
        return;
    }

    // Special handling for stoves.
    if (ingredients[selectedIngredientID]->getIngredientType() == StoveOff) {
        addIngredient(StoveOn, ingredients[selectedIngredientID]->getPosition());
        removeIngredient(selectedIngredientID);
        return;
    }
    if (ingredients[selectedIngredientID]->getIngredientType() == StoveOn) {
        addIngredient(StoveOff, ingredients[selectedIngredientID]->getPosition());
        removeIngredient(selectedIngredientID);
        return;
    }


    // Put the selected body into the selected variable.
    for (b2Body* body = world.GetBodyList();
         body != nullptr;
         body = body->GetNext()) {
        // This horrible piece of code converts the void pointer from
        // b2Body::GetUserData() to an int
        // https://stackoverflow.com/questions/30768714/properly-casting-a-void-to-an-integer-in-c
        int ingredientID = static_cast<int>(
            reinterpret_cast<intptr_t>(body->GetUserData()));

        // qDebug() << "Comparing selected ingredient" << selectedIngredientID << "against" << ingredientID;

        if (ingredientID == selectedIngredientID) {
            selected = body;
            qDebug() << "Selected b2Body of Ingredient ID:" << selectedIngredientID;
            // Call pointMoved to set recent mouse location.
            pointMoved(position);
            return;
        }
    }
}

void Model::pointMoved(QPointF position) {
    recentMouseLoc = position;
}

void Model::pointReleased() {
    selected = nullptr;
}

void Model::pauseGame(bool pausedState) {
    // Pause game, stop timer to stop game loop
    if (pausedState)
        timer.stop();
    else
        timer.start(FRAME_TIME);
}
