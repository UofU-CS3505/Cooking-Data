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
    HamHead,
    HamSlice,
    Knife,
    Ladel,
    LettuceHead,
    LettuceSlice,
    OatPacket,
    OatmealBowl,
    OatsBowl,
    Sandwich,
    SandwichTomato,
    SandwichTomatoLettuce,
    SandwichTomatoLettuceMeat,
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
