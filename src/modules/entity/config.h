//
// Created by Arjo Nagelhout on 30/12/2023.
//

#ifndef SHAPEREALITY_ENTITY_CONFIG_H
#define SHAPEREALITY_ENTITY_CONFIG_H

#include <cstdlib>

/**
 * @namespace entity
 * @brief simple entity component system (ECS) using sparse sets to store and iterateUsingStack over component data
 */
namespace entity
{
    using size_type = size_t;
    using EntityId = size_type;
}

#endif //SHAPEREALITY_ENTITY_CONFIG_H
