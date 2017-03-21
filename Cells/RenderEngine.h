#pragma once

#include "CellUnit.h"
#include <thread>

struct pixel;

enum class actionType 
{ 
    mouseClick 
};

class CRenderEngine
{
public:
    CRenderEngine();
    ~CRenderEngine();

    void start();
    void stop();
    void render();
    void actionHandle(actionType, const int& mouseX, const int& mouseY);

    void setHandles(HWND _hWnd, HDC _hDC);
    void setWorkingArea(const long& _screenWidth, const long& _screenHeight);
    void setUnitSizes(const int& _unitWidth, const int& _unitHeight);
    void setUnitColors(const COLORREF& _inactiveFill, const COLORREF& _inactiveBorder, const COLORREF& _activeFill, const COLORREF& _activeBorder);

    int getWidth() const;
    int getHeight() const;
    HDC getDC() const;

private:
    int upw;
    int uph;

    HWND hSourceWnd;
    HDC hSourceDC;
    HDC hWorkingDC;
    HBITMAP hBitmap;

    pixel* pixels;
    CCellUnit* units;

    long screenWidth;
    long screenHeight;

    int unitWidth;
    int unitHeight;

    COLORREF inactiveFill;
    COLORREF inactiveBorder;
    COLORREF activeFill;
    COLORREF activeBorder;

    std::thread renderThread;
};

