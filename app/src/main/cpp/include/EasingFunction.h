#pragma once

namespace argeo
{
    enum class EasingFunction : unsigned char
    {
        Linear = 0,
        SmoothStep = 1,
        EaseInBack = 2,
        EaseOutBack = 3,
        EaseInsine = 4,
        EaseOutsine = 5,
        Sine = 6,
        Repeating = 7
    };
}