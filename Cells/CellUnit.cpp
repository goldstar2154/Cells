#include "stdafx.h"
#include "CellUnit.h"
#include "Pixel.h"
#include "SettingsManager.h"

CCellUnit::CCellUnit()
    : width(0)
    , height(0)
    , posX(0)
    , posY(0)
    , isActive(false)
    , isNeedRedraw(true)
    , animationState(0)
{
}


CCellUnit::~CCellUnit()
{
}

void CCellUnit::changeState()
{
    isActive = !isActive;
    isNeedRedraw = true;
}

bool CCellUnit::needRender() const
{
    return isNeedRedraw;
}

void CCellUnit::render(pixel* _pixels)
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            pixel* current = &_pixels[posY * 1600 + posX + j * 1600 + i];
          
            if (true == isActive)
            {
                current->r = GetRValue(CSettingsManager::Instance().getActiveCellFill());
                current->g = GetGValue(CSettingsManager::Instance().getActiveCellFill());
                current->b = GetBValue(CSettingsManager::Instance().getActiveCellFill());
            }
            else
            {
                current->r = GetRValue(CSettingsManager::Instance().getInactiveCellFill());
                current->g = GetGValue(CSettingsManager::Instance().getInactiveCellFill());
                current->b = GetBValue(CSettingsManager::Instance().getInactiveCellFill());
            }
        }
    }

    isNeedRedraw = false;
}
