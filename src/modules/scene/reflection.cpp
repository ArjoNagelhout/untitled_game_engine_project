//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "reflection.h"

#include <scene/scene.h>

namespace scene
{
    void registerReflection(reflection::TypeInfoRegistry& r, reflection::JsonSerializer& jsonSerializer)
    {
        reflection::TypeInfoBuilder<Scene>("Scene")
            .property<&Scene::name>("name")
            .property<&Scene::entities>("entities")
            .emplace(r);
    }
}