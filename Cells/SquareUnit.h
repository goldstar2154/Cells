#pragma once

#include "BaseUnit.h"

class CSquareUnit : public CBaseUnit
{
public:
    CSquareUnit();
    ~CSquareUnit();

public:
    void render(pixel* _pixels) override;
    void changeState() override;
    void changeVisibility(const bool& _isVisible);

private:
    bool isVisible;
    long visibilityCounter;
};
