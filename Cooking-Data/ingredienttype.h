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
    EmptyBowl,
    EmptyPot,
    Ladel,
    OatPacket,
    OatmealBowl,
    OatsBowl,
    WaterLadel,
    WaterPitcher,
    WaterPot,
    // Put fire-related things at the end, alphabetically.
    Ember,
    Fire,
};

#endif // INGREDIENTTYPE_H
