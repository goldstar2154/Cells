#include "stdafx.h"
#include "RenderEngine.h"
#include "Pixel.h"
#include <stdio.h>
#include <iostream>
#include "SettingsManager.h"

CRenderEngine::CRenderEngine()
{
}


CRenderEngine::~CRenderEngine()
{
}

void CRenderEngine::setHandles(HWND _hWnd, HDC _hDC)
{
    hSourceWnd = _hWnd;
    hSourceDC = _hDC;
}

void CRenderEngine::setWorkingArea(const long& _screenWidth, const long& _screenHeight)
{
    screenHeight = _screenHeight;
    screenWidth = _screenWidth;
}

void CRenderEngine::setUnitSizes(const int& _unitWidth, const int& _unitHeight)
{
    unitWidth = _unitWidth;
    unitHeight = _unitHeight;
}

void CRenderEngine::setUnitColors(const COLORREF& _inactiveFill, const COLORREF& _inactiveBorder, const COLORREF& _activeFill, const COLORREF& _activeBorder)
{
    inactiveFill = _inactiveFill;
    inactiveBorder = _inactiveBorder;
    activeFill = _activeFill;
    activeBorder = _activeBorder;
}

int CRenderEngine::getWidth() const
{
    return screenWidth;
}

int CRenderEngine::getHeight() const
{
    return screenHeight;
}

HDC CRenderEngine::getDC() const
{
    return hWorkingDC;
}

void CRenderEngine::start()
{
    // calculating count of units
    upw = (screenWidth - 1) / (unitWidth + 1);
    uph = (screenHeight - 1) / (unitHeight + 1);

    hWorkingDC = CreateCompatibleDC(hSourceDC);

//    hWorkingDC = GetDC(hSourceWnd);

    SetDCBrushColor(hWorkingDC, RGB(120, 120, 120));

    LineTo(hWorkingDC, 50, 50);

    // initializing units
    units = new CCellUnit[upw*uph];


    for (int i = 0; i < upw; ++i)
    {
        for (int j = 0; j < uph; ++j)
        {
            CCellUnit* current = &units[i + j * upw];
            current->width = unitWidth;
            current->height = unitHeight;

            current->posX = i*unitWidth + 1 * (i + 1);
            current->posY = j*unitHeight + 1 * (j + 1);
        }
    }

    // Creating drawing bitmap   
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biWidth = screenWidth;
    bmi.bmiHeader.biHeight = -screenHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // last byte not used, 32 bit for alignment
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;
    bmi.bmiColors[0].rgbBlue = 0;
    bmi.bmiColors[0].rgbGreen = 0;
    bmi.bmiColors[0].rgbRed = 0;
    bmi.bmiColors[0].rgbReserved = 0;
    HDC hdc = GetDC(hSourceWnd);
    // Create DIB section to always give direct access to pixels
    hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
    DeleteDC(hdc);

    renderThread = std::thread([=] { render(); });
    renderThread.detach();
}

void CRenderEngine::render()
{
    for (;;)
    {
        for (int i = 0; i < this->upw; ++i)
        {
            for (int j = 0; j < uph; ++j)
            {
                CCellUnit* current = &units[i * uph + j];
                
                if (true == current->needRender())
                    current->render(pixels);
            }
        }
        
        hWorkingDC = CreateCompatibleDC(hSourceDC);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hWorkingDC, hBitmap);

        BitBlt(hSourceDC, 0, 0, screenWidth, screenHeight, hWorkingDC, 0, 0, SRCCOPY);

        SelectObject(hWorkingDC, hbmOld);

        UpdateWindow(hSourceWnd);

        Sleep(25);

    }

}

void CRenderEngine::actionHandle(actionType, const int& mouseX, const int& mouseY)
{
    // allocating object

    // check line hit
    if (0 == (mouseX % (CSettingsManager::Instance().getCellWidth() + 1)) || 0 == (mouseY % (CSettingsManager::Instance().getCellHeight() + 1)))
        return;

    // detect hitted object
    int uX = (mouseX - 1) / (CSettingsManager::Instance().getCellWidth() + 1);
    int uY = (mouseY - 1) / (CSettingsManager::Instance().getCellHeight() + 1);

    CCellUnit* current = &units[uX + uY * upw];
    current->changeState();

}