#include "model.h"

#include <QObject>
#include <QDebug>


Model::Model()
    : timer(),
    world(b2World(b2Vec2(0.0f, 10.0f))){

    // THIS IS TEST CODE, WE WILL EDIT THIS LATER AS NEEDED
    ////////////////////////////

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 60.0f);

    b2Vec2 gravity(0.0f, 10.0f);
    this->world.SetGravity(gravity);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = this->world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);



    connect(&timer, &QTimer::timeout, this, &Model::updateWorld);
    timer.start(16);

    // Let view create the ground
    QTimer::singleShot(50, this, [&](){emit makeGroundInView(b2Vec2(0.0f, 60.0f), 100, 20);});
}
Model::~Model(){

}

void Model::addObject(float x, float y, float width, float height){
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
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.2f;
    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);
    body->SetType(b2_dynamicBody);
}
void Model::removeObject(qsizetype index){
    world.DestroyBody(&world.GetBodyList()[index]);
}

void Model::updateWorld()
{
    // About 60fps, 10 iterations per collision check
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    world.Step(timeStep, velocityIterations, positionIterations);


    // Loop through every body, send signal for any dynamic one
    int dynamicCount = 0;
    for(b2Body* body = world.GetBodyList(); body != nullptr; body = body->GetNext())
    {
        if(body->GetType() == b2_dynamicBody)
        {
            emit objectUpdated(dynamicCount, body);
            dynamicCount++;
        }
    }
}

