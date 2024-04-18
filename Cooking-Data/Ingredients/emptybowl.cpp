#include "emptybowl.h"

EmptyBowl::EmptyBowl(QPoint pos)
    : Ingredient(pos, QSize(6, 4), 0,
                 QPixmap(":/ingredients/assets/images/sprites/EmptyBowl.png")) {

}
