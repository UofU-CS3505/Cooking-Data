#include <QObject>
#include <QDebug>

#include "ingredienttype.h"
#include "model.h"

Model::Model()
    : timer(),
    world(b2World(b2Vec2(0.0f, 9.8f))){

    // THIS IS TEST CODE, WE WILL EDIT THIS LATER AS NEEDED
    ////////////////////////////

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 1.1f);
    b2Body* groundBody = this->world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(2.0f, 0.1f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define static walls
    b2BodyDef leftWallBodyDef;
    leftWallBodyDef.position.Set(-0.1f, 0.5f);
    b2Body* leftBody = this->world.CreateBody(&leftWallBodyDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(0.1f, 1.0f);
    leftBody->CreateFixture(&leftWallBox, 0.0f);

    b2BodyDef rightWallBodyDef;
    rightWallBodyDef.position.Set(1.7f, 0.5f);
    b2Body* rightBody = this->world.CreateBody(&rightWallBodyDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(0.1f, 1.0f);
    rightBody->CreateFixture(&rightWallBox, 0.0f);

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
        qMakePair(WaterPot, Fire),
        qMakePair(QVector<IngredientType> { BoilingWaterPot, Fire }, 0));
    combinations.insert(
        qMakePair(BoilingWaterPot, Ladel),
        qMakePair(QVector<IngredientType> { BoilingWaterPot, WaterLadel }, 0));
    combinations.insert(
        qMakePair(EmptyBowl, OatPacket),
        qMakePair(QVector<IngredientType> { OatsBowl, OatPacket }, 0));
    combinations.insert(
        qMakePair(OatsBowl, WaterLadel),
        qMakePair(QVector<IngredientType> { OatmealBowl, Ladel }, 0));
}

Model::~Model() {
    // How to delete maps?
    // qDeleteAll(ingredients);
    // qDeleteAll(combinations);
    // qDeleteAll(ingredientToBody);
}

void Model::addIngredient(IngredientType type, QPointF position) {
    Ingredient* ingredient = createIngredient(type, position, 0);
    qDebug() << "Ingredient made with ID" << ingredient->getID();
    ingredients.insert(ingredient->getID(), ingredient);
    qDebug() << "Inserted ingredient with ID" << ingredients[ingredient->getID()]->getID() << "into map with key" << ingredient->getID();
    addIngredientToWorld(*ingredient);
    qDebug() << "box2d made";
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

    if (type == Fire)
        return new Ingredient(Fire, QSizeF(0.1, 0.15), 0.1,
                              QPixmap(":/ingredients/assets/images/sprites/Fire.png"),
                              position, angle);

    return new Ingredient();
}

b2Body* Model::addIngredientToWorld(const Ingredient& ingredient) {
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.position.Set(ingredient.getPosition().x(),
                         ingredient.getPosition().y());
    bodyDef.angle = ingredient.getAngle();
    b2Body* body = this->world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(ingredient.getDimensions().width() / 2,
                        ingredient.getDimensions().height() / 2);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = ingredient.getWeight()
                         / ingredient.getDimensions().width()
                         / ingredient.getDimensions().height();

    // Override the default friction.
    fixtureDef.friction = 0.8f;
    fixtureDef.restitution = 0.1f;
    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);
    body->SetType(b2_dynamicBody);

    // Set corresponding ingredient's ID
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

        if (ingredientIDofBody == ingredientID) {
            world.DestroyBody(body);
            isBodyDestroyed = true;
            break;
        }
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

bool Model::combine(int i1, int i2) {
    // Check if both ingredients are in the map of ingredients.
    if (!(ingredients.contains(i1) && ingredients.contains(i2)))
        return false;

    QPair<IngredientType, IngredientType> pair
        = qMakePair(ingredients[i1]->getIngredientType(),
                    ingredients[i2]->getIngredientType());

    for (int pairType = 0; pairType < 2; pairType++) {
        if (combinations.contains(pair)) {
            // Spawn the resulting Ingredients.
            for (int i = 0; i < combinations[pair].first.size(); i++) {
                if ((i == 0 && pairType == 0) || (i != 0 && pairType == 1))
                    addIngredient(combinations[pair].first[i],
                                  ingredients[i1]->getPosition());
                else
                    addIngredient(combinations[pair].first[i],
                                  ingredients[i2]->getPosition());

                if (combinations[pair].first[i] == winCondition)
                    qDebug() << "VICTORY ROYALE";
            }

            removeIngredient(i1);
            removeIngredient(i2);

            qDebug() << "Combined";
            return true;
        }

        // Invert pair in case the collision was detected in reverse.
        pair = qMakePair(ingredients[i2]->getIngredientType(),
                         ingredients[i1]->getIngredientType());
    }

    // Not combined.
    return false;
}

void Model::createWorld(int level) {
    addIngredient(WaterPitcher, QPointF(0.2, 0));
    addIngredient(OatPacket, QPointF(0.4, 0));
    addIngredient(EmptyPot, QPointF(0.6, 0));
    addIngredient(Ladel, QPointF(0.8, 0));
    addIngredient(Fire, QPointF(1.0, 0));
    for (int i = 0; i < 4; i++)
        addIngredient(EmptyBowl, QPointF((std::rand() % 200) / 100.0, 0));

    if (level == 2) {

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

        // Limit combines to once per frame... for reasons.
        if (combine(i1ID, i2ID))
            break;
    }

    emit frameBegan();

    // Loop through every body, update their position and angle, send signal for any dynamic one.
    for (b2Body* body = world.GetBodyList();
         body != nullptr;
         body = body->GetNext()) {
        if(body->GetType() == b2_dynamicBody) {
            // This horrible piece of code converts the void pointer from
            // b2Body::GetUserData() to an int
            // https://stackoverflow.com/questions/30768714/properly-casting-a-void-to-an-integer-in-c
            int ingredientID = static_cast<int>(
                reinterpret_cast<intptr_t>(body->GetUserData()));

            // qDebug() << "Checking if Ingredient" << ingredientID << "is in the map";
            // If the ingredient was not found in the map, continue the loop.
            if (!ingredients.contains(ingredientID))
                continue;

            Ingredient& ingredient = *ingredients[ingredientID];

            ingredient.setPosition(QPointF(body->GetPosition().x,
                                           body->GetPosition().y));
            ingredient.setAngle(qRadiansToDegrees(body->GetAngle()));

            emit ingredientUpdated(ingredient);
        }
    }

    emit frameEnded();
}

void Model::deleteWorld(){
    for (b2Body* body = world.GetBodyList();
         body != nullptr;
         body = body->GetNext())
        if (body->GetType() == b2_dynamicBody)
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

        if (ingredientID == selectedIngredientID && body->GetType() == b2_dynamicBody) {
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
