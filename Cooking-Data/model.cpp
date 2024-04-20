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

}

void Model::addIngredient(IngredientType type, QPointF position) {
    qDebug() << "ID creating...";
    int* ID = new int(latestIngredientID);
    qDebug() << "ID made: " << *ID;
    Ingredient ingredient = createIngredient(type, position, 0, ID);
    qDebug() << "Ingredient made";
    activeIngredients.append(ingredient);
    qDebug() << "added to activeingreentsts";
    addBox2DObject(position.x(), position.y(),
                   ingredient.getDimensions().width(),
                   ingredient.getDimensions().height(),
                   qDegreesToRadians(ingredient.getAngle()), ID);
    qDebug() << "box2d made";
    latestIngredientID++;
    qDebug() << "ID plus plus: " << latestIngredientID;

    // ingredientToBody.insert(ingredient, addBox2DObject(position.x(), position.y(),
    //                         ingredient.getDimensions().width(),
    //                         ingredient.getDimensions().height(),
    //                         qDegreesToRadians(ingredient.getAngle())));
    // qDebug() << ingredientToBody[ingredient]->GetPosition().x;
}

b2Body* Model::addBox2DObject(float x, float y, float width, float height, float angle, int* ingredientID) {
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
    body->SetUserData(ingredientID);
    return body;
}

void Model::removeBox2DObject(qsizetype index) {
    world.DestroyBody(&world.GetBodyList()[index]);
}

Ingredient Model::createIngredient(IngredientType ingType, QPointF position, double angle, int* ID) {
    if (ingType == BoilingWaterPot)
        return Ingredient(BoilingWaterPot, position, QSizeF(8, 7), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/BoilingWaterPot.png"
                          ), ID);

    if (ingType == EmptyBowl)
        return Ingredient(EmptyBowl, position, QSizeF(6, 4), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/EmptyBowl.png"
                              ), ID);

    if (ingType == EmptyPot)
        return Ingredient(EmptyPot, position, QSizeF(8, 5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/EmptyPot.png"
                              ), ID);

    if (ingType == Ladel)
        return Ingredient(Ladel, position, QSizeF(4, 6.5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/Ladel.png"
                              ), ID);

    if (ingType == OatmealBowl)
        return Ingredient(OatmealBowl, position, QSizeF(6, 8.5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/OatmealBowl.png"
                              ), ID);

    if (ingType == OatsBowl)
        return Ingredient(OatsBowl, position, QSizeF(6, 4), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/OatsBowl.png"
                              ), ID);

    if (ingType == WaterLadel)
        return Ingredient(WaterLadel, position, QSizeF(4, 6.5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/WaterLadel.png"
                              ), ID);

    if (ingType == WaterPitcher)
        return Ingredient(WaterPitcher, position, QSizeF(7.5, 8.5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/WaterPitcher.png"
                              ), ID);

    if (ingType == WaterPot)
        return Ingredient(WaterPot, position, QSizeF(8, 5), angle,
                          QPixmap(
                              ":/ingredients/assets/images/sprites/WaterPot.png"
                              ), ID);


    return Ingredient();
}

bool Model::combine(const Ingredient& i1, const Ingredient& i2) {
    if (!(activeIngredients.contains(i1) && activeIngredients.contains(i2)))
        return false;

    QPair<IngredientType, IngredientType> potential1
        = qMakePair(i1.getIngredientType(), i2.getIngredientType());
    QPair<IngredientType, IngredientType> potential2
        = qMakePair(i1.getIngredientType(), i2.getIngredientType());

    // TODO - what QSize, angle, Pixmap should we pass in?
    if (combinations.contains(potential1)) {
        activeIngredients.remove(activeIngredients.indexOf(i1));
        activeIngredients.remove(activeIngredients.indexOf(i2));
        int* ID = new int(latestIngredientID);
        Ingredient newIngredient(combinations[potential1], i1.getPosition(),
                                 QSize(10, 10), 0.0, QPixmap(), ID);
        activeIngredients.append(newIngredient);
        latestIngredientID++;
        return true;
    }

    // TODO - what QSize, angle, Pixmap should we pass in?
    if (combinations.contains(potential2)) {
        activeIngredients.remove(activeIngredients.indexOf(i1));
        activeIngredients.remove(activeIngredients.indexOf(i2));
        int* ID = new int(latestIngredientID);
        Ingredient newIngredient(combinations[potential2], i1.getPosition(),
                                 QSize(10, 10), 0.0, QPixmap(), ID);
        activeIngredients.append(newIngredient);
        return true;
        latestIngredientID++;
    }

    return false;
}

void Model::createWorld() {
    for (int i = 0; i < 10; i++)
        addIngredient(EmptyBowl, QPointF(5, 0));

    emit worldCreated(activeIngredients);

    qDebug() << "world created";
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

    // Loop through every body, send signal for any dynamic one
    int dynamicCount = 0;
    for(b2Body* body = world.GetBodyList();
        body != nullptr;
        body = body->GetNext()) {
        if(body->GetType() == b2_dynamicBody) {

            Ingredient bodyIngredient;
            //QSet<Ingredient>::iterator ingredient;
            for (Ingredient& ingredient : activeIngredients) {
                if(body->GetUserData() == ingredient.getID())
                {
                    //bodyIngredient = ingredient;
                    //qDebug() << "ingX " << ingredient.getPosition().x() << " | ingY " << ingredient.getPosition().y();
                    ingredient.setPosition(QPointF(body->GetPosition().x,
                                                       body->GetPosition().y));
                    ingredient.setAngle(qRadiansToDegrees(body->GetAngle()));
                    emit ingredientUpdated(dynamicCount, ingredient);
                    dynamicCount++;
                }
            }

            //qDebug() << "ingX " << bodyIngredient.getPosition().x() << " | ingY " << bodyIngredient.getPosition().y();
            // qDebug() << "bodX " << body->GetPosition().x << " | bodY " << body->GetPosition().y;
            // bodyIngredient.setPosition(QPointF(body->GetPosition().x,
            //                                               body->GetPosition().y));
            // bodyIngredient.setAngle(qRadiansToDegrees(body->GetAngle()));
            // emit ingredientUpdated(dynamicCount, bodyIngredient);
            // dynamicCount++;
        }
    }
}

void Model::pointPressed(QPointF position) {
    // Iterate every object to find which one is selected.
    // If nothing is selected, return.
    int* selectedIngredientID = new int(-1);

    for (const Ingredient& item : activeIngredients){
        double x1 = item.getPosition().x()
                    - item.getDimensions().width()/2;
        double y1 = item.getPosition().y()
                    - item.getDimensions().height()/2;
        double x2 = item.getPosition().x()
                    + item.getDimensions().width()/2;
        double y2 = item.getPosition().y()
                    + item.getDimensions().height()/2;

         qDebug() << "mouse x " << position.x() << " | mouse y " << position.y();
         qDebug() << "x1 " << x1 << " | x2 " << x2;
         qDebug() << "y1 " << y1 << " | y2 " << y2;
        if (x1 <= position.x() && x2 >= position.x()
            && y1 <= position.y() && y2 >= position.y()) {
            //qDebug() << item.getIngredientType();
            selectedIngredientID = item.getID();
            break;
        }
    }

    // If the index is still -1, it means nothing was selected.
    if (*selectedIngredientID == -1) {
        selected = nullptr;
        return;
    }

    // Put the selected body into the selected variable.
    ///
    /// \brief dynamicCount counts all the dynamic objects in the world (excludes the static ones)
    ///
    for(b2Body* body = world.GetBodyList();
         body != nullptr;
         body = body->GetNext()) {
        if(body->GetUserData() == selectedIngredientID && body->GetType() == b2_dynamicBody) {
            selected = body;
            qDebug() << "Selected ingredient";
            return;
        }
    }

    pointMoved(position);
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
