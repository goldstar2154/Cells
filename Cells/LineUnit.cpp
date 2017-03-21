#include "stdafx.h"
#include "LineUnit.h"
#include "Pixel.h"
#include "SettingsManager.h"

CLineUnit::CLineUnit()
    : CBaseUnit()
    , isVisible(true)
{
}

CLineUnit::~CLineUnit()
{
}

void CLineUnit::render(pixel* _pixels)
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

void CLineUnit::changeVisibility(const bool& _isVisible)
{
    isVisible = _isVisible;
}
