#include <QObject>
#include <QDebug>

#include "model.h"
#include "Ingredients/emptybowl.h"

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
}

Model::~Model() {

}

void Model::addIngredient(QPointF position) {
    Ingredient emptyBowl = EmptyBowl(position, 0);
    ingredients.append(emptyBowl);
    addBox2DObject(position.x(), position.y(),
              emptyBowl.getDimensions().width(),
              emptyBowl.getDimensions().height(),
              qDegreesToRadians(emptyBowl.getOrientation()));
}


void Model::addBox2DObject(float x, float y, float width, float height, float angle) {
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
}

void Model::removeBox2DObject(qsizetype index) {
    world.DestroyBody(&world.GetBodyList()[index]);
}

void Model::createWorld() {
    for (int i = 0; i < 10; i++)
        addIngredient(QPointF(5, 0));

    emit worldCreated(ingredients);

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
            ingredients[dynamicCount].setPosition(QPointF(body->GetPosition().x,
                                                          body->GetPosition().y));
            ingredients[dynamicCount].setOrientation(qRadiansToDegrees(body->GetAngle()));
            emit objectUpdated(dynamicCount, ingredients.at(dynamicCount));
            dynamicCount++;
        }
    }
}

void Model::pointPressed(QPointF position) {
    // Iterate every object to find which one is selected.
    // If nothing is selected, return.
    int selectedIngredientIndex = -1;

    for (int i = 0; i < ingredients.size(); i++){
        auto item = ingredients[i];
        double x1 = item.getPosition().x();
        double y1 = item.getPosition().y();
        double x2 = item.getPosition().x()
                    + item.getDimensions().width();
        double y2 = item.getPosition().y()
                    + item.getDimensions().height();

        qDebug() << "mouse x " << position.x() << " | mouse y " << position.y();
        qDebug() << "x1 " << x1 << " | x2 " << x2;
        qDebug() << "y1 " << y1 << " | y2 " << y2;
        if (x1 <= position.x() && x2 >= position.x()
            && y1 <= position.y() && y2 >= position.y()) {
            selectedIngredientIndex = ingredients.size() - i - 1;
            qDebug() << "Item " << selectedIngredientIndex << " selected.";
            break;
        }
    }

    // If the index is still -1, it means nothing was selected.
    if (selectedIngredientIndex == -1) {
        selected = nullptr;
        return;
    }

    // Put the selected body into the selected variable.
    ///
    /// \brief dynamicCount counts all the dynamic objects in the world (excludes the static ones)
    ///
    int dynamicCount = 0;
    for(b2Body* body = world.GetBodyList();
        dynamicCount <= selectedIngredientIndex;
        body = body->GetNext()) {
        if(dynamicCount == selectedIngredientIndex && body->GetType() == b2_dynamicBody) {
            selected = body;
            return;
        } else if(body->GetType() == b2_dynamicBody)
            dynamicCount++;
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

void Model::modelUpdated(int index, Ingredient rect) {
    // idk what we are planning to use this for? is this a slot or a method?
}

