#include <QObject>
#include <QDebug>

#include "ingredienttype.h"
#include "model.h"

Model::Model()
    : timer(),
    world(b2World(b2Vec2(0.0f, 15.0f))){

    // THIS IS TEST CODE, WE WILL EDIT THIS LATER AS NEEDED
    ////////////////////////////

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 60.0f);

    b2Vec2 gravity(0.0f, 20.0f);
    this->world.SetGravity(gravity);

    // Define static walls
    b2Body* groundBody = this->world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(100.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef leftWallBodyDef;
    leftWallBodyDef.position.Set(-10.0f, 30.0f);
    b2Body* leftBody = this->world.CreateBody(&leftWallBodyDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(10.0f, 100.0f);
    leftBody->CreateFixture(&leftWallBox, 0.0f);

    b2BodyDef rightWallBodyDef;
    rightWallBodyDef.position.Set(90.0f, 30.0f);
    b2Body* rightBody = this->world.CreateBody(&rightWallBodyDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(10.0f, 100.0f);
    rightBody->CreateFixture(&rightWallBox, 0.0f);

    // Start the timer.
    connect(&timer, &QTimer::timeout, this, &Model::updateWorld);
    timer.start(16);

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
    combinations.insert(qMakePair(WaterPitcher, EmptyPot), WaterPot);
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
    addBox2DObject(position.x(), position.y(),
                   ingredient->getDimensions().width(),
                   ingredient->getDimensions().height(),
                   qDegreesToRadians(ingredient->getAngle()),
                   ingredient->getID());
    qDebug() << "box2d made";
}

b2Body* Model::addBox2DObject(float x, float y, float width, float height, float angle, int ingredientID) {
    // THIS IS A TEMP TO CREATE SIMPLE BOX

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.angle = angle;
    b2Body* body = this->world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(width / 2, height / 2);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

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
        reinterpret_cast<void *>(static_cast<intptr_t>(ingredientID)));
    return body;
}

void Model::removeBox2DObject(qsizetype index) {
    world.DestroyBody(&world.GetBodyList()[index]);
}

Ingredient* Model::createIngredient(IngredientType ingType, QPointF position, double angle) {
    if (ingType == BoilingWaterPot)
        return new Ingredient(BoilingWaterPot, position, QSizeF(8, 7), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/BoilingWaterPot.png"
                                  ));

    if (ingType == EmptyBowl)
        return new Ingredient(EmptyBowl, position, QSizeF(6, 4), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/EmptyBowl.png"
                                  ));

    if (ingType == EmptyPot)
        return new Ingredient(EmptyPot, position, QSizeF(8, 5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/EmptyPot.png"
                                  ));

    if (ingType == Ladel)
        return new Ingredient(Ladel, position, QSizeF(4, 6.5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/Ladel.png"
                                  ));

    if (ingType == OatmealBowl)
        return new Ingredient(OatmealBowl, position, QSizeF(6, 8.5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/OatmealBowl.png"
                                  ));

    if (ingType == OatsBowl)
        return new Ingredient(OatsBowl, position, QSizeF(6, 4), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/OatsBowl.png"
                                  ));

    if (ingType == WaterLadel)
        return new Ingredient(WaterLadel, position, QSizeF(4, 6.5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/WaterLadel.png"
                                  ));

    if (ingType == WaterPitcher)
        return new Ingredient(WaterPitcher, position, QSizeF(7.5, 8.5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/WaterPitcher.png"
                                  ));

    if (ingType == WaterPot)
        return new Ingredient(WaterPot, position, QSizeF(8, 5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/WaterPot.png"
                                  ));


    return new Ingredient();
}

bool Model::combine(const Ingredient& i1, const Ingredient& i2) {
    // Check if both ingredients are in the map of ingredients.
    if (!(ingredients.contains(i1.getID()) && ingredients.contains(i2.getID())))
        return false;

    QPair<IngredientType, IngredientType> potential1
        = qMakePair(i1.getIngredientType(), i2.getIngredientType());
    QPair<IngredientType, IngredientType> potential2
        = qMakePair(i1.getIngredientType(), i2.getIngredientType());

    // TODO - what QSize, angle, Pixmap should we pass in?
    if (combinations.contains(potential1)) {
        ingredients.remove(i1.getID());
        ingredients.remove(i2.getID());
        // Destroy the body too
        for (b2Body* body = world.GetBodyList();
             body != nullptr;
             body = body->GetNext()) {
            // This horrible piece of code converts the void pointer from
            // b2Body::GetUserData() to an int
            // https://stackoverflow.com/questions/30768714/properly-casting-a-void-to-an-integer-in-c
            int ingredientID = static_cast<int>(
                reinterpret_cast<intptr_t>(body->GetUserData()));

            if (ingredientID == i1.getID())
                world.DestroyBody(body);
            if (ingredientID == i2.getID())
                world.DestroyBody(body);
        }
        addIngredient(combinations[potential1], i1.getPosition());
        qDebug() << "COMBINED: Case1";
        return true;
    }

    // TODO - what QSize, angle, Pixmap should we pass in?
    if (combinations.contains(potential2)) {
        ingredients.remove(i1.getID());
        ingredients.remove(i2.getID());
        // Destroy the body too
        for (b2Body* body = world.GetBodyList();
             body != nullptr;
             body = body->GetNext()) {
            // This horrible piece of code converts the void pointer from
            // b2Body::GetUserData() to an int
            // https://stackoverflow.com/questions/30768714/properly-casting-a-void-to-an-integer-in-c
            int ingredientID = static_cast<int>(
                reinterpret_cast<intptr_t>(body->GetUserData()));

            if (ingredientID == i1.getID())
                world.DestroyBody(body);
            if (ingredientID == i2.getID())
                world.DestroyBody(body);
        }
        addIngredient(combinations[potential2], i1.getPosition());
        qDebug() << "COMBINED: Case2";
        return true;
    }
    return false;
}

void Model::createWorld() {
    addIngredient(WaterPitcher, QPointF(5, 0));
    addIngredient(EmptyPot, QPointF(15, 10));

    emit worldCreated(ingredients.values());

    qDebug() << "World created";
}

void Model::updateWorld() {
    // About 60fps, 10 iterations per collision check
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    if(selected != nullptr){
        // SPRING
        float distanceX = recentMouseLoc.x() - selected->GetPosition().x;
        float distanceY = recentMouseLoc.y() - selected->GetPosition().y;
        float velocityX = distanceX * selected->GetMass() * 2;
        float velocityY = distanceY * selected->GetMass() * 2;
        b2Vec2 force = b2Vec2(velocityX, velocityY);
        selected->ApplyForceToCenter(force, true);

        // float velocityX = distanceX / timeStep;
        // float velocityY = distanceY / timeStep;
        // float accelX = (velocityX - oldVX) / timeStep;
        // float accelY = (velocityY - oldVY) / timeStep;
        // b2Vec2 force = b2Vec2(accelX * selected->GetMass(), accelY * selected->GetMass());
        // selected->ApplyForceToCenter(force, true);
        // oldVX = velocityX;
        // oldVX = velocityY;

        // float distanceX = recentMouseLoc.x() - selected->GetPosition().x;
        // float distanceY = recentMouseLoc.y() - selected->GetPosition().y;
        // float velocityX = distanceX / timeStep;
        // float velocityY = distanceY / timeStep;
        // b2Vec2 force = b2Vec2(velocityX * selected->GetMass(), velocityY * selected->GetMass());
        // selected->ApplyForceToCenter(force, true);

        // float mass = selected->GetMass();
        // qDebug() << velocityY;
        // if(velocityY > world.GetGravity().y)
        //     velocityY -= world.GetGravity().y;
        //selected->SetLinearVelocity(b2Vec2(velocityX / selected->GetMass(), velocityY / selected->GetMass()));
    }

    world.Step(timeStep, velocityIterations, positionIterations);

    // Check for combinations
    for(b2Contact* collision = world.GetContactList();
        collision != nullptr;
        collision = collision->GetNext()) {
        // This horrible piece of code converts the void pointer from
        // b2Body::GetUserData() to an int
        // https://stackoverflow.com/questions/30768714/properly-casting-a-void-to-an-integer-in-c
        int i1ID = static_cast<int>(
            reinterpret_cast<intptr_t>(collision->GetFixtureA()->GetBody()->GetUserData()));
        int i2ID = static_cast<int>(
            reinterpret_cast<intptr_t>(collision->GetFixtureB()->GetBody()->GetUserData()));

        // If the ingredient was not found in the map, continue the loop.
        if (!(ingredients.contains(i1ID) && ingredients.contains(i2ID)))
            continue;

        Ingredient& i1 = *ingredients[i1ID];
        Ingredient& i2 = *ingredients[i2ID];
        combine(i1, i2);
    }

    // Loop through every body, update their position and angle, send signal for any dynamic one.
    int dynamicCount = 0;
    for(b2Body* body = world.GetBodyList();
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

            emit ingredientUpdated(dynamicCount, ingredient);
            dynamicCount++;
        }
    }
}

void Model::deleteWorld(){
    for(b2Body* body = world.GetBodyList();
         body != nullptr;
         body = body->GetNext()) {
        if(body->GetType() == b2_dynamicBody)
            world.DestroyBody(body);
    }
    ingredients.clear();
}

void Model::pointPressed(QPointF position) {
    // Iterate every object to find which one is selected.
    // If nothing is selected, return.
    int selectedIngredientID = -1;

    for (auto [key, value] : ingredients.asKeyValueRange()) {
        double x1 = value->getPosition().x()
                    - value->getDimensions().width() / 2;
        double y1 = value->getPosition().y()
                    - value->getDimensions().height() / 2;
        double x2 = value->getPosition().x()
                    + value->getDimensions().width() / 2;
        double y2 = value->getPosition().y()
                    + value->getDimensions().height() / 2;
        
        qDebug() << "mouse x " << position.x() << " | mouse y " << position.y();
        qDebug() << "x1 " << x1 << " | x2 " << x2;
        qDebug() << "y1 " << y1 << " | y2 " << y2;
        if (x1 <= position.x() && x2 >= position.x()
            && y1 <= position.y() && y2 >= position.y()) {
            qDebug() << "ID" << key << "; actual ID" << value->getID();
            selectedIngredientID = value->getID();
            qDebug() << "Selected Ingredient ID:" << selectedIngredientID;
            break;
        }
    }

    // If the index is still -1, it means nothing was selected.
    if (selectedIngredientID == -1) {
        selected = nullptr;
        return;
    }

    // Put the selected body into the selected variable.
    ///
    /// \brief dynamicCount counts all the dynamic objects in the world (excludes the static ones)
    ///
    for (b2Body* body = world.GetBodyList();
         body != nullptr;
         body = body->GetNext()) {
        // This horrible piece of code converts the void pointer from
        // b2Body::GetUserData() to an int
        // https://stackoverflow.com/questions/30768714/properly-casting-a-void-to-an-integer-in-c
        int ingredientID = static_cast<int>(
            reinterpret_cast<intptr_t>(body->GetUserData()));

        qDebug() << "Comparing selected ingredient" << selectedIngredientID << "against" << ingredientID;

        if(ingredientID == selectedIngredientID && body->GetType() == b2_dynamicBody) {
            selected = body;
            qDebug() << "Selected ingredient";
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

void Model::pauseGame(bool pausedState){
    // Pause game, stop timer to stop game loop
    if(pausedState)
        timer.stop();
    else
        timer.start(16);
}
