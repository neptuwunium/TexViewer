#pragma once

#include <QtWidgets/qspinbox.h>

class Pow2Spinbox : public QSpinBox
{
public:
    virtual void stepBy(int steps) override;
};
