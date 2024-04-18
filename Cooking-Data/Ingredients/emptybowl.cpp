#include "emptybowl.h"

EmptyBowl::EmptyBowl(QPointF newPosition, double angle)
    : Ingredient(newPosition, QSize(6, 4), angle,
                 QPixmap(":/ingredients/assets/images/sprites/EmptyBowl.png")) {

}
