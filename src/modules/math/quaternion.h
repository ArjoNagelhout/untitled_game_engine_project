//
// Created by Arjo Nagelhout on 16/11/2023.
//

#ifndef SHAPEREALITY_QUATERNION_H
#define SHAPEREALITY_QUATERNION_H

#include "vector.h"

namespace math
{
    // can be used to represent rotations
    template<typename Type>
    struct Quaternion final
    {
        constexpr explicit Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        float x;
        float y;
        float z;
        float w;

        constexpr ~Quaternion() = default;

        //
        const static Quaternion identity;

        //
        [[nodiscard]] constexpr Vector<3, Type> toEulerInRadians() const;

        //
        [[nodiscard]] constexpr Vector<3, Type> toEulerInDegrees() const;

        //
        [[nodiscard]] constexpr static Quaternion createFromEulerInRadians(Vector3 eulerAngles);

        //
        [[nodiscard]] constexpr static Quaternion createFromEulerInDegrees(Vector3 eulerAngles);
    };
}

#endif //SHAPEREALITY_QUATERNION_H
