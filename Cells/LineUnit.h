#pragma once

#include "BaseUnit.h"

class CLineUnit : public CBaseUnit
{
public:
    CLineUnit();
    ~CLineUnit();

public:
    void render(pixel* _pixels) override;
    void changeVisibility(const bool& _isVisible);

private:
    bool isVisible;
};