#include "stdafx.h"
#include "SquareUnit.h"
#include "Pixel.h"
#include "SettingsManager.h"

CSquareUnit::CSquareUnit()
    : CBaseUnit()
    , isVisible(true)
    , visibilityCounter(0)
{
}

CSquareUnit::~CSquareUnit()
{
}

void CSquareUnit::render(pixel* _pixels)
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            pixel* current = &_pixels[y * CSettingsManager::Instance().getScreenWidth() + x + j * CSettingsManager::Instance().getScreenWidth() + i];

            if (false == isVisible)
            {
                current->r = GetRValue(CSettingsManager::Instance().getActiveCellFill());
                current->g = GetGValue(CSettingsManager::Instance().getActiveCellFill());
                current->b = GetBValue(CSettingsManager::Instance().getActiveCellFill());
            }
            else
            {
                if (true == isActive)
                {
                    current->r = GetRValue(CSettingsManager::Instance().getActiveCellBorder());
                    current->g = GetGValue(CSettingsManager::Instance().getActiveCellBorder());
                    current->b = GetBValue(CSettingsManager::Instance().getActiveCellBorder());
                }
                else
                {
                    current->r = GetRValue(CSettingsManager::Instance().getInactiveCellBorder());
                    current->g = GetGValue(CSettingsManager::Instance().getInactiveCellBorder());
                    current->b = GetBValue(CSettingsManager::Instance().getInactiveCellBorder());
                }
            }
        }
    }

    isNeedRedraw = false;
}

void CSquareUnit::changeState()
{
    bool bOldIsActive = isActive;

    if (visibilityCounter > 0)
        isActive = true;
    else
    if (visibilityCounter < 0)
        isActive = false;
    else
        isActive = !isActive;

    isNeedRedraw = true;
}

void CSquareUnit::changeVisibility(const bool& _isVisible)
{
    if (true == _isVisible)
    {
        if (true == isActive)
            --visibilityCounter;
        else
            ++visibilityCounter;
    }
    else
    {
        if (true == isActive)
            ++visibilityCounter;
    }

    if (visibilityCounter == 4)
        isVisible = false;
    else
        isVisible = true;
}
