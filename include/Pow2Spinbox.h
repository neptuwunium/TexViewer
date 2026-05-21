#pragma once

#include <QSpinBox>
#include <bit>
#include <QGuiApplication>

class Pow2Spinbox : public QSpinBox
{
public:
    virtual void stepBy(int steps) override;
};
