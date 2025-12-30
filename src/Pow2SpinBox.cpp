#include <bit>
#include <QGuiApplication>
#include "Pow2Spinbox.h"

static int
nearestPowerOfTwo(int v)
{
    if (v <= 1)
        return 2;

    int lower = 1 << (std::bit_width(static_cast<unsigned>(v)) - 1);
    int upper = lower << 1;

    return (v - lower < upper - v) ? lower : upper;
}

void
Pow2Spinbox::stepBy(int steps)
{
    bool shiftHeld = QGuiApplication::keyboardModifiers() & Qt::ShiftModifier;

    int v = value();

    if (shiftHeld) {
        const int stepShiftJump = minimum();
        int value = v + steps * stepShiftJump;
        if (value < stepShiftJump)
            value = stepShiftJump;
        else if (stepShiftJump > 1)
            value -= value % stepShiftJump;

        setValue(value);
        return;
    }

    int snapped = nearestPowerOfTwo(v);
    int index = std::bit_width(static_cast<unsigned>(snapped)) - 2;
    index += steps;
    index = std::clamp(index, 0, 13);

    int value = 1 << (index + 1);
    if (value < 4)
        value = 4;

    setValue(value);
}
