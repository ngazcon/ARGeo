#pragma once

namespace argeo
{
    enum class Axis : char
    {
        X = 0,
        Y = 1,
        Z = 2,
    };

    const mat4d Y_UP_TO_Z_UP = mat_rotate_x(PI_OVER_TWO);
    const mat4d Z_UP_TO_Y_UP = mat_rotate_x(-PI_OVER_TWO);

    const mat4d X_UP_TO_Z_UP = mat_rotate_y(-PI_OVER_TWO);
    const mat4d Z_UP_TO_X_UP = mat_rotate_y(PI_OVER_TWO);

    const mat4d X_UP_TO_Y_UP = mat_rotate_z(PI_OVER_TWO);
    const mat4d Y_UP_TO_X_UP = mat_rotate_z(-PI_OVER_TWO);
}