#pragma once

#include "BaseUnit.h"

class CCellUnit : public CBaseUnit
{
public:
    CCellUnit();
    ~CCellUnit();

public:
    void render(pixel* _pixels) override;
    void changeState() override;

    long animationProgress;
};
