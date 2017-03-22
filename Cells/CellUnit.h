#pragma once

#include "BaseUnit.h"

class CCellUnit : public CBaseUnit
{
public:
    CCellUnit();
    ~CCellUnit();

    void render(pixel* _pixels) override;
    void changeState() override;

private:
    long animationProgress;
};
