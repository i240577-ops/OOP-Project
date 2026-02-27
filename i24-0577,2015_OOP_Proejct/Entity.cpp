#include "Entity.h"

Entity::Entity()
    : position(0.0f, 0.0f),
    health(100),
    maxHealth(100) {
}

Entity::~Entity() = default;
