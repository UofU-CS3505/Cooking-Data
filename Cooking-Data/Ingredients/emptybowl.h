#ifndef EMPTYBOWL_H
#define EMPTYBOWL_H

#include "ingredient.h"

class EmptyBowl : public Ingredient {
public:
    EmptyBowl(QPointF newPosition, double angle);
};

#endif // EMPTYBOWL_H
