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
    BreadLoaf,
    BreadSlice,
    EmptyBowl,
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
    BreadTomato,
    BreadTomatoHam,
    BreadTomatoHamLettuce,
    BreadTomatoHamLettuceBread,
    Tomato,
    TomatoSlice,
    WaterLadel,
    WaterPitcher,
    WaterPot,
    // Put fire-related things at the end, alphabetically.
    Ember,
    Fire,
};

#endif // INGREDIENTTYPE_H
