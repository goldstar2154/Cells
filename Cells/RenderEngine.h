#pragma once

struct pixel;
class CCellUnit;
class CLineUnit;
class CSquareUnit;

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
    void handleAction(actionType, const int& mouseX, const int& mouseY);
    void setHandles(HWND _hWnd, HDC _hDC);
    HDC getDC() const;

private:
    int upw;
    int uph;

    HWND hSourceWnd;
    HDC hSourceDC;
    HDC hWorkingDC;
    HBITMAP hBitmap;
    pixel* pixels;

    CCellUnit* cellUnits;
    CLineUnit* horLines;
    CLineUnit* vertLines;
    CSquareUnit* squareUnits;

    std::thread renderThread;

    volatile bool needStop;
};

