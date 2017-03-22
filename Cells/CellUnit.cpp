#include "stdafx.h"
#include "CellUnit.h"
#include "Pixel.h"
#include "SettingsManager.h"

CCellUnit::CCellUnit()
    : CBaseUnit()
    , animationProgress(11)
{
}

CCellUnit::~CCellUnit()
{
}

void CCellUnit::render(pixel* _pixels)
{
    if (animationStatus::stop == animation)
    {
        isNeedRedraw = false;
        return;
    }

    if (animationStatus::playForward == animation)
        ++animationProgress;
    else
    if (animationStatus::playBack == animation)
        --animationProgress;

    for (int i = 0; i < width  - width * (11 - animationProgress) / 10; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            pixel* current = &_pixels[y * CSettingsManager::Instance().getScreenWidth() + x + j * CSettingsManager::Instance().getScreenWidth() + i];

            if (true == isActive || animation == animationStatus::playForward || animation == animationStatus::playBack)
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
    
    for (int i = width - width * (11 - animationProgress) / 10; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            pixel* current = &_pixels[y * CSettingsManager::Instance().getScreenWidth() + x + j * CSettingsManager::Instance().getScreenWidth() + i];

            if (true == isActive || animation == animationStatus::playForward || animation == animationStatus::playBack)
            {
                current->r = GetRValue(CSettingsManager::Instance().getInactiveCellFill());
                current->g = GetGValue(CSettingsManager::Instance().getInactiveCellFill());
                current->b = GetBValue(CSettingsManager::Instance().getInactiveCellFill());
            }
            else
            {
                current->r = GetRValue(CSettingsManager::Instance().getActiveCellFill());
                current->g = GetGValue(CSettingsManager::Instance().getActiveCellFill());
                current->b = GetBValue(CSettingsManager::Instance().getActiveCellFill());
            }
        }
    }

    // ending animation
    if (animationStatus::firstDraw == animation 
        || animationProgress == 11
        || animationProgress == 1)
        animation = animationStatus::stop;

    isNeedRedraw = true;
}

void CCellUnit::changeState()
{
    if (animation != animationStatus::stop)
    {
        if (animation == animationStatus::playBack)
            animation = animationStatus::playForward;
        else
        if (animation == animationStatus::playForward)
            animation = animationStatus::playBack;
    }
    else
    {
        if (false == isActive)
        {
            animation = animationStatus::playForward;
            animationProgress = 1;
        }
        else
        {
            animation = animationStatus::playBack;
            animationProgress = 10;
        }
    }

    CBaseUnit::changeState();
}
