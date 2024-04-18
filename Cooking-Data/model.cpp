#include <QObject>
#include <QDebug>

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

void Model::addObject(float x, float y, float width, float height) {
    // THIS IS A TEMP TO CREATE SIMPLE BOX

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    b2Body* body = this->world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(width, height);

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
void Model::removeObject(qsizetype index) {
    world.DestroyBody(&world.GetBodyList()[index]);
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
            emit objectUpdated(dynamicCount, body);
            dynamicCount++;
        }
    }
}

void Model::objectClicked(int index, float x, float y) {
    int dynamicCount = 0;
    for(b2Body* body = world.GetBodyList();
        dynamicCount <= index;
        body = body->GetNext()) {
        if(dynamicCount == index && body->GetType() == b2_dynamicBody) {
            // body->ApplyForceToCenter(
            //     b2Vec2(
            //         (x - body->GetPosition().x) * body->GetMass(),
            //         (y - body->GetPosition().y) * body->GetMass()),
            //     true);
            recentMouseLoc = QPoint(x, y);
            selected = body;
            return;
        }
        else if(body->GetType() == b2_dynamicBody)
            dynamicCount++;
    }
}

void Model::pauseGame(bool pausedState){
    // Pause game, stop timer to stop game loop
    if(pausedState)
        timer.stop();
    else
        timer.start(16);
}

void Model::objectReleased() {
    selected = nullptr;
}

void Model::modelUpdated(int index, Ingredient rect) {
    // idk what we are planning to use this for? is this a slot or a method?
}

