#pragma once

#include <limits>

namespace argeo
{
    enum class SceneMode : unsigned char
    {
        Scene2D  = 0,
        Scene3D  = 1,
        SceneCV  = 2,
        Morphing = 3
    };

    static double get_morph_time(const SceneMode& value)
    {
        if (value == SceneMode::Scene3D)
        {
            return 1.0;
        }
        else if(value == SceneMode::Morphing)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        return 0.0;
    }
}
