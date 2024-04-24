#ifndef INGREDIENTTYPE_H
#define INGREDIENTTYPE_H

enum IngredientType {
    None,
    // Put stationary things at the front, alphabetically.
    Plank,
    StoveOff,
    StoveOn,
    // Alphabetically sort actual Ingredients.
    BoilingWaterPot,
    BoilingWaterRicePot,
    BreadLoaf,
    BreadSlice,
    BreadTomato,
    BreadTomatoHam,
    BreadTomatoHamLettuce,
    BreadTomatoHamLettuceBread,
    Broccoli,
    BroccoliChopped,
    Chicken,
    ChickenChopped,
    ChickenPan,
    ChickenCookedPan,
    EmptyBowl,
    EmptyPan,
    EmptyPlate,
    EmptyPot,
    Ham,
    HamSlice,
    Knife,
    Ladel,
    Lettuce,
    LettuceSlice,
    OatPacket,
    OatmealBowl,
    OatsBowl,
    Rice,
    RicePlate,
    RiceChickenPlate,
    RiceChickenBroccoliPlate,
    Tomato,
    TomatoSlice,
    WaterLadel,
    WaterPitcher,
    WaterPot,
    WaterRicePot,
    // Put fire-related things at the end, alphabetically.
    Ember,
    Fire,
};

#endif // INGREDIENTTYPE_H
