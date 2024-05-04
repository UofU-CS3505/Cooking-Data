#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QtMath>

#include "ingredienttype.h"
#include "model.h"

Model::Model()
    : timer(),
    world(b2World(b2Vec2(0.0f, 9.8f))){

    // Start the timer.
    connect(&timer, &QTimer::timeout, this, &Model::updateWorld);

    // Let view create the ground and background.

    // Define all valid combinations.
    // Fire decay and merging.
    combinations.insert(
        qMakePair(Fire, None),
        qMakePair(QVector<IngredientType> { Fire, Ember }, 2000));
    combinations.insert(
        qMakePair(Ember, Ember),
        qMakePair(QVector<IngredientType> { Fire }, 0));
    combinations.insert(
        qMakePair(Ember, None),
        qMakePair(QVector<IngredientType> { }, 5000));

    // LEVEL 1 COMBOS
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
        qMakePair(StoveOn, EmptyBowl),
        qMakePair(QVector<IngredientType> { StoveOn, EmptyBowl, Ember }, 2000));
    combinations.insert( // this wouldn't do anything unless ladel is selected
        qMakePair(StoveOn, Ladel),
        qMakePair(QVector<IngredientType> { StoveOn, Ladel, Ember }, 2000));
    combinations.insert(
        qMakePair(StoveOn, OatsBowl),
        qMakePair(QVector<IngredientType> { StoveOn, EmptyBowl, Ember }, 1000));
    combinations.insert(
        qMakePair(StoveOn, OatPacket),
        qMakePair(QVector<IngredientType> { StoveOn, OatPacket, Ember }, 1000));
    combinations.insert(
        qMakePair(EmptyBowl, OatPacket),
        qMakePair(QVector<IngredientType> { OatsBowl, OatPacket }, 0));
    combinations.insert(
        qMakePair(WaterPitcher, EmptyPot),
        qMakePair(QVector<IngredientType> { WaterPitcher, WaterPot }, 0));
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
        qMakePair(BoilingWaterPot, None),
        qMakePair(QVector<IngredientType> { WaterPot }, 10000));
    combinations.insert(
        qMakePair(BoilingWaterPot, Ladel),
        qMakePair(QVector<IngredientType> { BoilingWaterPot, WaterLadel }, 0));
    combinations.insert(
        qMakePair(OatsBowl, WaterLadel),
        qMakePair(QVector<IngredientType> { OatmealBowl, Ladel }, 0));
    combinations.insert(
        qMakePair(OatsBowl, Fire),
        qMakePair(QVector<IngredientType> { EmptyBowl, Ember }, 1000));
    combinations.insert(
        qMakePair(OatPacket, Fire),
        qMakePair(QVector<IngredientType> { Fire, Fire }, 1900)); // has to be shorter than fire decay
    combinations.insert(
        qMakePair(OatPacket, Ember),
        qMakePair(QVector<IngredientType> { OatPacket, Ember, Ember }, 1000));
    combinations.insert(
        qMakePair(WaterLadel, Fire),
        qMakePair(QVector<IngredientType> { Ladel, Ember }, 0));
    combinations.insert(
        qMakePair(WaterLadel, Ember),
        qMakePair(QVector<IngredientType> { Ladel }, 0));

    // LEVEL 2 COMBOS
    combinations.insert(
        qMakePair(Knife, BreadLoaf),
        qMakePair(QVector<IngredientType> { Knife, BreadLoaf, BreadSlice }, 750));
    combinations.insert(
        qMakePair(Knife, Ham),
        qMakePair(QVector<IngredientType> { Knife, Ham, HamSlice }, 750));
    combinations.insert(
        qMakePair(Knife, Lettuce),
        qMakePair(QVector<IngredientType> { Knife, Lettuce, LettuceSlice }, 750));
    combinations.insert(
        qMakePair(Knife, Tomato),
        qMakePair(QVector<IngredientType> { Knife, Tomato, TomatoSlice }, 750));
    combinations.insert(
        qMakePair(TomatoSlice, BreadSlice),
        qMakePair(QVector<IngredientType> { BreadTomato }, 100));
    combinations.insert(
        qMakePair(HamSlice, BreadTomato),
        qMakePair(QVector<IngredientType> { BreadTomatoHam }, 100));
    combinations.insert(
        qMakePair(LettuceSlice, BreadTomatoHam),
        qMakePair(QVector<IngredientType> { BreadTomatoHamLettuce }, 100));
    combinations.insert(
        qMakePair(BreadTomatoHamLettuce, BreadSlice),
        qMakePair(QVector<IngredientType> { BreadTomatoHamLettuceBread }, 100));

    // LEVEL 3 COMBOS
    combinations.insert(
        qMakePair(Knife, Broccoli),
        qMakePair(QVector<IngredientType> { Knife, BroccoliChopped }, 750));
    combinations.insert(
        qMakePair(Knife, Chicken),
        qMakePair(QVector<IngredientType> { Knife, ChickenChopped }, 750));
    combinations.insert(
        qMakePair(WaterPot, Rice),
        qMakePair(QVector<IngredientType> { WaterRicePot, Rice }, 0));
    combinations.insert(
        qMakePair(BoilingWaterPot, Rice),
        qMakePair(QVector<IngredientType> { WaterRicePot, Rice }, 0));
    combinations.insert(
        qMakePair(StoveOn, WaterRicePot),
        qMakePair(QVector<IngredientType> { StoveOn, BoilingWaterRicePot }, 5000));
    combinations.insert(
        qMakePair(StoveOn, BoilingWaterRicePot),
        qMakePair(QVector<IngredientType> { StoveOn, EmptyPot, Fire  }, 5000));
    combinations.insert(
        qMakePair(StoveOn, EmptyPan),
        qMakePair(QVector<IngredientType> { StoveOn, EmptyPan, Ember }, 2000));
    combinations.insert(
        qMakePair(StoveOn, ChickenPan),
        qMakePair(QVector<IngredientType> { StoveOn, ChickenCookedPan }, 5000));
    combinations.insert(
        qMakePair(StoveOn, ChickenCookedPan),
        qMakePair(QVector<IngredientType> { StoveOn, EmptyPan, Fire  }, 5000));
    combinations.insert(
        qMakePair(StoveOn, Rice),
        qMakePair(QVector<IngredientType> { StoveOn, Rice, Ember }, 1000));
    combinations.insert(
        qMakePair(StoveOn, Broccoli),
        qMakePair(QVector<IngredientType> { StoveOn, Broccoli, Ember }, 1000));
    combinations.insert(
        qMakePair(StoveOn, BroccoliChopped),
        qMakePair(QVector<IngredientType> { StoveOn, BroccoliChopped, Ember }, 1000));
    combinations.insert(
        qMakePair(StoveOn, Chicken),
        qMakePair(QVector<IngredientType> { StoveOn, Chicken, Ember }, 1000));
    combinations.insert(
        qMakePair(StoveOn, ChickenChopped),
        qMakePair(QVector<IngredientType> { StoveOn, ChickenChopped, Ember }, 1000));
    combinations.insert(
        qMakePair(StoveOn, EmptyPlate),
        qMakePair(QVector<IngredientType> { StoveOn, EmptyPlate, Ember }, 1000));
    combinations.insert(
        qMakePair(StoveOn, RicePlate),
        qMakePair(QVector<IngredientType> { StoveOn, EmptyPlate }, 1000));
    combinations.insert(
        qMakePair(StoveOn, RiceChickenPlate),
        qMakePair(QVector<IngredientType> { StoveOn, EmptyPlate }, 1000));
    combinations.insert(
        qMakePair(StoveOn, RiceChickenBroccoliPlate),
        qMakePair(QVector<IngredientType> { StoveOn, EmptyPlate }, 1000));
    combinations.insert(
        qMakePair(EmptyPan, ChickenChopped),
        qMakePair(QVector<IngredientType> { ChickenPan }, 0));
    combinations.insert(
        qMakePair(EmptyPlate, BoilingWaterRicePot),
        qMakePair(QVector<IngredientType> { RicePlate, EmptyPot }, 0));
    combinations.insert(
        qMakePair(RicePlate, ChickenCookedPan),
        qMakePair(QVector<IngredientType> { RiceChickenPlate, EmptyPan }, 0));
    combinations.insert(
        qMakePair(RicePlate, Fire),
        qMakePair(QVector<IngredientType> { EmptyPlate, Ember }, 1000));
    combinations.insert(
        qMakePair(RiceChickenPlate, BroccoliChopped),
        qMakePair(QVector<IngredientType> { RiceChickenBroccoliPlate }, 0));
    combinations.insert(
        qMakePair(RiceChickenPlate, Fire),
        qMakePair(QVector<IngredientType> { EmptyPlate, Ember }, 1000));
    combinations.insert(
        qMakePair(RiceChickenBroccoliPlate, Fire),
        qMakePair(QVector<IngredientType> { EmptyPlate, Ember }, 1000));
    combinations.insert(
        qMakePair(Broccoli, Fire),
        qMakePair(QVector<IngredientType> { Fire, Fire }, 1900)); // has to be shorter than fire decay
    combinations.insert(
        qMakePair(Broccoli, Ember),
        qMakePair(QVector<IngredientType> { Broccoli, Ember, Ember }, 1000));
    combinations.insert(
        qMakePair(BroccoliChopped, Fire),
        qMakePair(QVector<IngredientType> { Fire, Fire }, 1900)); // has to be shorter than fire decay
    combinations.insert(
        qMakePair(BroccoliChopped, Ember),
        qMakePair(QVector<IngredientType> { BroccoliChopped, Ember, Ember }, 1000));
    combinations.insert(
        qMakePair(Chicken, Fire),
        qMakePair(QVector<IngredientType> { Fire, Fire }, 1900)); // has to be shorter than fire decay
    combinations.insert(
        qMakePair(Chicken, Ember),
        qMakePair(QVector<IngredientType> { Chicken, Ember, Ember }, 1000));
    combinations.insert(
        qMakePair(ChickenChopped, Fire),
        qMakePair(QVector<IngredientType> { Fire, Fire }, 1900)); // has to be shorter than fire decay
    combinations.insert(
        qMakePair(ChickenChopped, Ember),
        qMakePair(QVector<IngredientType> { ChickenChopped, Ember, Ember }, 1000));
    combinations.insert(
        qMakePair(Rice, Fire),
        qMakePair(QVector<IngredientType> { Fire, Fire }, 1900)); // has to be shorter than fire decay
    combinations.insert(
        qMakePair(Rice, Ember),
        qMakePair(QVector<IngredientType> { Rice, Ember, Ember }, 1000));

    // Add all the active tools.
    tools.append(Ladel);
    tools.append(WaterLadel);
    tools.append(WaterPitcher);
    tools.append(Knife);
}

Model::~Model() {
    // How to delete maps?
    qDeleteAll(ingredients);
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

    //Other
    if (type == Plank)
        return new Ingredient(Plank, QSizeF(0.2, 0.05), 0,
                              QPixmap(":/ingredients/assets/images/sprites/Plank.png"),
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
        return new Ingredient(Fire, QSizeF(0.1, 0.15), 0.2, 0.4, 0.4,
                              QPixmap(":/ingredients/assets/images/sprites/Fire.png"),
                              position, 0);

    if (type == Ember)
        return new Ingredient(Ember, QSizeF(0.0375, 0.0375), 0.1, 0.1, 0.6,
                              QPixmap(":/ingredients/assets/images/sprites/Ember.png"),
                              position, angle);

    //Level 1
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

    //Level 2
    if (type == BreadLoaf)
        return new Ingredient(BreadLoaf, QSizeF(0.1125, 0.1125), 1,
                              QPixmap(":/ingredients/assets/images/sprites/BreadLoaf.png"),
                              position, angle);

    if (type == BreadSlice)
        return new Ingredient(BreadSlice, QSizeF(0.1125, 0.075), 0.2,
                              QPixmap(":/ingredients/assets/images/sprites/BreadSlice.png"),
                              position, angle);

    if (type == BreadTomato)
        return new Ingredient(BreadTomato, QSizeF(0.1125, 0.1), 0.4,
                              QPixmap(":/ingredients/assets/images/sprites/BreadTomato.png"),
                              position, angle);

    if (type == BreadTomatoHam)
        return new Ingredient(BreadTomatoHam, QSizeF(0.1125, 0.1375), 0.6,
                              QPixmap(":/ingredients/assets/images/sprites/BreadTomatoHam.png"),
                              position, angle);

    if (type == BreadTomatoHamLettuce)
        return new Ingredient(BreadTomatoHamLettuce, QSizeF(0.1125, 0.175), 0.8,
                              QPixmap(":/ingredients/assets/images/sprites/BreadTomatoHamLettuce.png"),
                              position, angle);

    if (type == BreadTomatoHamLettuceBread)
        return new Ingredient(BreadTomatoHamLettuceBread, QSizeF(0.1125, 0.2125), 1,
                              QPixmap(":/ingredients/assets/images/sprites/BreadTomatoHamLettuceBread.png"),
                              position, angle);

    if (type == Ham)
        return new Ingredient(Ham, QSizeF(0.2, 0.15), 1, 0.4, 0.6,
                              QPixmap(":/ingredients/assets/images/sprites/Ham.png"),
                              position, angle);

    if (type == HamSlice)
        return new Ingredient(HamSlice, QSizeF(0.1125, 0.0625), 0.2, 0.4, 0.6,
                              QPixmap(":/ingredients/assets/images/sprites/HamSlice.png"),
                              position, angle);

    if (type == Knife)
        return new Ingredient(Knife, QSizeF(0.05, 0.1625), 0.2,
                              QPixmap(":/ingredients/assets/images/sprites/Knife.png"),
                              position, angle);

    if (type == Lettuce)
        return new Ingredient(Lettuce, QSizeF(0.125, 0.125), 0.5, 0.6, 0.4,
                              QPixmap(":/ingredients/assets/images/sprites/Lettuce.png"),
                              position, angle);

    if (type == LettuceSlice)
        return new Ingredient(LettuceSlice, QSizeF(0.1125, 0.075), 0.2, 0.6, 0.4,
                              QPixmap(":/ingredients/assets/images/sprites/LettuceSlice.png"),
                              position, angle);

    if (type == Tomato)
        return new Ingredient(Tomato, QSizeF(0.0875, 0.0875), 0.5, 0.6, 0.4,
                              QPixmap(":/ingredients/assets/images/sprites/Tomato.png"),
                              position, angle);

    if (type == TomatoSlice)
        return new Ingredient(TomatoSlice, QSizeF(0.1125, 0.0625), 0.2, 0.6, 0.4,
                              QPixmap(":/ingredients/assets/images/sprites/TomatoSlice.png"),
                              position, angle);

    //Level 3
    if (type == EmptyPan)
        return new Ingredient(EmptyPan, QSizeF(0.1875, 0.125), 2,
                              QPixmap(":/ingredients/assets/images/sprites/Pan.png"),
                              position, angle);

    if (type == Chicken)
        return new Ingredient(Chicken, QSizeF(0.175, 0.175), 0.5,
                              QPixmap(":/ingredients/assets/images/sprites/Chicken.png"),
                              position, angle);

    if (type == Broccoli)
        return new Ingredient(Broccoli, QSizeF(0.1125,0.1125), 0.5,
                              QPixmap(":/ingredients/assets/images/sprites/Brocoli.png"),
                              position, angle);

    if (type == Rice)
        return new Ingredient(Rice, QSizeF(0.15, 0.175), 4,
                              QPixmap(":/ingredients/assets/images/sprites/RiceBag.png"),
                              position, angle);

    if (type == ChickenChopped)
        return new Ingredient(ChickenChopped, QSizeF(12*.0125, 7*.0125), 0.2,
                              QPixmap(":/ingredients/assets/images/sprites/CutChicken.png"),
                              position, angle);

    if (type == ChickenPan)
        return new Ingredient(ChickenPan, QSizeF(15*.0125, 10*.0125), 2.2,
                              QPixmap(":/ingredients/assets/images/sprites/ChickenPan.png"),
                              position, angle);

    if (type == ChickenCookedPan)
        return new Ingredient(ChickenCookedPan, QSizeF(15*.0125, 16*.0125), 2.2,
                              QPixmap(":/ingredients/assets/images/sprites/FinsihedChickenPan.png"),
                              position, angle);

    if (type == BroccoliChopped)
        return new Ingredient(BroccoliChopped, QSizeF(7*.0125, 7*.0125), 0.2,
                              QPixmap(":/ingredients/assets/images/sprites/CutBrocoli.png"),
                              position, angle);

    if (type == WaterRicePot)
        return new Ingredient(WaterRicePot, QSizeF(16*.0125, 10*.0125), 4.5,
                              QPixmap(":/ingredients/assets/images/sprites/RicePot.png"),
                              position, angle);

    if (type == BoilingWaterRicePot)
        return new Ingredient(BoilingWaterRicePot, QSizeF(16*.0125, 16*.0125), 4.5,
                              QPixmap(":/ingredients/assets/images/sprites/FinishedRicePot.png"),
                              position, angle);

    if (type == EmptyPlate)
        return new Ingredient(EmptyPlate, QSizeF(11*.0125, 6*.0125), 0.5,
                              QPixmap(":/ingredients/assets/images/sprites/Plate.png"),
                              position, angle);

    if (type == RicePlate)
        return new Ingredient(RicePlate, QSizeF(11*.0125, 6*.0125), 1,
                              QPixmap(":/ingredients/assets/images/sprites/RicePlate.png"),
                              position, angle);

    if (type == RiceChickenPlate)
        return new Ingredient(RiceChickenPlate, QSizeF(11*.0125, 6*.0125), 1.2,
                              QPixmap(":/ingredients/assets/images/sprites/RiceChickenPlate.png"),
                              position, angle);

    if (type == RiceChickenBroccoliPlate)
        return new Ingredient(RiceChickenBroccoliPlate, QSizeF(11*.0125, 6*.0125), 1.4,
                              QPixmap(":/ingredients/assets/images/sprites/RiceChickenBroccoliPlate.png"),
                              position, angle);

    return new Ingredient(position);
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
    fixtureDef.friction = ingredient.getFriction();
    fixtureDef.restitution = ingredient.getRestitution();
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

bool Model::tryCombine(int i1, int i2) {
    // Check if both Ingredients (combination) are in the map of ingredients.
    // If only one Ingredient (transformation), only check if that one is in the
    // map.
    if (!(ingredients.contains(i1) && ingredients.contains(i2))
        && (i2 == -1 && !ingredients.contains(i1)))
        return false;



    // If either Ingredient is a tool, check if the they are selected.
    if (i2 != -1 && !areIngredientsNotToolsOrSelectedTool(i1, i2))
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
            QTimer::singleShot(1000, this, [&](){emit winConditionMet();});
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
            QTimer::singleShot(1000, this, [&](){emit winConditionMet();});
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
            QTimer::singleShot(1000, this, [&](){emit winConditionMet();});
    } else if (i2 != -1 && combinations[typePair].first.size() == 1)
        // Is a combination but there is only one output.
        removeSuccessful = removeSuccessful && removeIngredient(i2);


    qDebug() << "Combined and removal" << removeSuccessful;
    emit comboCompleted();
    return removeSuccessful;
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

bool Model::areIngredientsNotToolsOrSelectedTool(int i1, int i2) {
    // Check if either Ingredient is a tool.
    if (!tools.contains(ingredients[i1]->getIngredientType())
        && !tools.contains(ingredients[i2]->getIngredientType()))
        return true;

    // Check if anything is selected.
    if (!selected)
        return false;

    int selectedID = static_cast<int>(
        reinterpret_cast<intptr_t>(selected->GetUserData()));
    // Check if selected is an Ingredient in the world (it should be)?
    if (!ingredients.contains(selectedID))
        return false;

    IngredientType selectedType =
        ingredients[selectedID]->getIngredientType();
    // Check if selected is one of the Ingredient in the collision.
    if (selectedType != ingredients[i1]->getIngredientType()
        && selectedType != ingredients[i2]->getIngredientType())
        return false;
    // Check if selected is a Tool.
    if (!tools.contains(selectedType))
        return false;

    return true;
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
        int i1 = static_cast<int>(reinterpret_cast<intptr_t>(
            collision->GetFixtureA()->GetBody()->GetUserData()));
        int i2 = static_cast<int>(reinterpret_cast<intptr_t>(
            collision->GetFixtureB()->GetBody()->GetUserData()));

        // If either Ingredient is not in the map, continue the loop.
        if (!(ingredients.contains(i1) && ingredients.contains(i2)))
            continue;

        // Try both possible order of combinations in case the collision was
        // detected in reverse.
        // Limit combines to once per frame... for reasons.
        if (tryCombine(i1, i2) || tryCombine(i2, i1))
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

    // Add shelf.
    addIngredient(Plank, QPointF(0.1, 0.6));
    addIngredient(Plank, QPointF(0.3, 0.6));
    addIngredient(Plank, QPointF(0.5, 0.6));
    addIngredient(Plank, QPointF(1.47, 0.6));
    addIngredient(Plank, QPointF(1.67, 0.6));
    addIngredient(Plank, QPointF(1.87, 0.6));

    // Add Ingredients.
    if (level == 1) {
        addIngredient(StoveOff, QPointF(0.99, 1.1));

        addIngredient(OatPacket, QPointF(0.2, 0));
        addIngredient(WaterPitcher, QPointF(0.4, 0));

        addIngredient(EmptyPot, QPointF(1.5, 0));
        addIngredient(EmptyBowl, QPointF(1.7, 0));
        addIngredient(Ladel, QPointF(1.9, 0));

        winCondition = OatmealBowl;
    } else if (level == 2) {
        addIngredient(Knife, QPointF(1, 1.1));

        addIngredient(BreadLoaf, QPointF(0.2, 0));
        addIngredient(Ham, QPointF(0.4, 0));

        addIngredient(Tomato, QPointF(1.6, 0));
        addIngredient(Lettuce, QPointF(1.8, 0));

        winCondition = BreadTomatoHamLettuceBread;
    } else if (level == 3) {
        // More shelf.
        addIngredient(Plank, QPointF(0.1, 0.3));
        addIngredient(Plank, QPointF(0.3, 0.3));
        addIngredient(Plank, QPointF(0.5, 0.3));
        addIngredient(Plank, QPointF(1.47, 0.3));
        addIngredient(Plank, QPointF(1.67, 0.3));
        addIngredient(Plank, QPointF(1.87, 0.3));

        addIngredient(StoveOff, QPointF(0.99, 1.1));

        addIngredient(WaterPitcher, QPointF(0.3, 0));

        addIngredient(EmptyPot, QPointF(1.5, 0));
        addIngredient(Knife, QPointF(1.7, 0));
        addIngredient(EmptyPan, QPointF(1.9, 0));

        addIngredient(Chicken, QPointF(0.1, 0.4));
        addIngredient(Broccoli, QPointF(0.3, 0.4));
        addIngredient(Rice, QPointF(0.5, 0.4));

        addIngredient(EmptyPlate, QPointF(1.7, 0.4));

        winCondition = RiceChickenBroccoliPlate;
    } else {
        // Just else makes this a fail safe is something went wrong
        winCondition = OatmealBowl;
    }

    timer.start(FRAME_TIME);
    qDebug() << "World created";
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
    for (b2Body* body = world.GetBodyList();
         body != nullptr;
         body = body->GetNext()) {
        if (body->GetFixtureList()->TestPoint(b2Vec2(position.x(), position.y()))) {
            selectedIngredientID = static_cast<int>(
                reinterpret_cast<intptr_t>(body->GetUserData()));
        }
    }

    // If the index is still -1 or somehow not in the list of Ingredients,
    // it means nothing was selected.
    if (selectedIngredientID == -1
        || !ingredients.contains(selectedIngredientID)) {
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
