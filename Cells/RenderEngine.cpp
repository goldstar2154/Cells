#include "stdafx.h"
#include "RenderEngine.h"
#include "Pixel.h"
#include "SettingsManager.h"
#include "CellUnit.h"
#include "LineUnit.h"
#include "SquareUnit.h"

CRenderEngine::CRenderEngine()
    : upw(0)
    , uph(0)
    , hSourceWnd(nullptr)
    , hSourceDC(nullptr)
    , hWorkingDC(nullptr)
    , hBitmap(nullptr)
    , pixels(nullptr)
    , cellUnits(nullptr)
    , horLines(nullptr)
    , vertLines(nullptr)
    , squareUnits(nullptr)
    , needStop(false)
{
}


CRenderEngine::~CRenderEngine()
{
    DeleteDC(hWorkingDC);
    DeleteObject(hBitmap);

    delete[] squareUnits;
    delete[] vertLines;
    delete[] horLines;
    delete[] cellUnits;
}

void CRenderEngine::setHandles(HWND _hWnd, HDC _hDC)
{
    hSourceWnd = _hWnd;
    hSourceDC = _hDC;
}

HDC CRenderEngine::getDC() const
{
    return hWorkingDC;
}

void CRenderEngine::start()
{
    hWorkingDC = CreateCompatibleDC(hSourceDC);

    // calculating count of units
    upw = (CSettingsManager::Instance().getScreenWidth() - CSettingsManager::Instance().getBorderWidth()) / (CSettingsManager::Instance().getCellWidth() + CSettingsManager::Instance().getBorderWidth());
    uph = (CSettingsManager::Instance().getScreenHeight() - CSettingsManager::Instance().getBorderWidth()) / (CSettingsManager::Instance().getCellHeight() + CSettingsManager::Instance().getBorderWidth());

    // creating & initializing units
    // cells
    cellUnits = new CCellUnit[upw * uph];
    for (int i = 0; i < upw; ++i)
    {
        for (int j = 0; j < uph; ++j)
        {
            CCellUnit* current = &cellUnits[i + j * upw];

            current->setSizes(CSettingsManager::Instance().getCellWidth(), CSettingsManager::Instance().getCellHeight());
            current->setPos(i * CSettingsManager::Instance().getCellWidth() + CSettingsManager::Instance().getBorderWidth() * (i + 1), 
                            j * CSettingsManager::Instance().getCellHeight() + CSettingsManager::Instance().getBorderWidth() * (j + 1));
        }
    }
    // lines
    // horizontal lines
    horLines = new CLineUnit[upw * (uph + 1)];
    for (int i = 0; i < upw; ++i)
    {
        for (int j = 0; j < uph + 1; j++)
        {
            CLineUnit* current = &horLines[i + j * upw];

            current->setSizes(CSettingsManager::Instance().getCellWidth(), CSettingsManager::Instance().getBorderWidth());
            current->setPos(i * (CSettingsManager::Instance().getCellWidth() + CSettingsManager::Instance().getBorderWidth()) + CSettingsManager::Instance().getBorderWidth(),
                j * (CSettingsManager::Instance().getCellHeight() + CSettingsManager::Instance().getBorderWidth()));
        }
    }
    // vertical Lines
    vertLines = new CLineUnit[(upw + 1) * uph];
    for (int i = 0; i < upw + 1; ++i)
    {
        for (int j = 0; j < uph; j++)
        {
            CLineUnit* current = &vertLines[i + j * (upw + 1)];

            current->setSizes(CSettingsManager::Instance().getBorderWidth(), CSettingsManager::Instance().getCellHeight());
            current->setPos(i * (CSettingsManager::Instance().getCellWidth() + CSettingsManager::Instance().getBorderWidth()),
                j * (CSettingsManager::Instance().getCellHeight() + CSettingsManager::Instance().getBorderWidth()) + CSettingsManager::Instance().getBorderWidth());
        }
    }
    // squares
    squareUnits = new CSquareUnit[(upw + 1) * (uph + 1)];
    for (int i = 0; i < upw + 1; ++i)
    {
        for (int j = 0; j < uph + 1; ++j)
        {
            CSquareUnit* current = &squareUnits[i + j * (upw + 1)];

            current->setSizes(CSettingsManager::Instance().getBorderWidth(), CSettingsManager::Instance().getBorderWidth());
            current->setPos(i * (CSettingsManager::Instance().getCellWidth() + CSettingsManager::Instance().getBorderWidth()),
                j * (CSettingsManager::Instance().getCellHeight() + CSettingsManager::Instance().getBorderWidth()));
        }
    }

    // Creating drawing bitmap for direct memory access
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biWidth = CSettingsManager::Instance().getScreenWidth();
    bmi.bmiHeader.biHeight = -CSettingsManager::Instance().getScreenHeight();   // minus for reverting array from top to bottom
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
    
    hBitmap = CreateDIBSection(hSourceDC, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);

    // main render thread, always run
    renderThread = std::thread([=] { render(); });
    renderThread.detach();
}

void CRenderEngine::stop()
{
    TerminateThread(renderThread.native_handle(), 1);
}

template<typename T>
void renderUnits(T* _array, const long& _size, pixel* _pixels)
{
    for (auto i = 0; i < _size; ++i)
    {
        T* cell = &_array[i];

        if (true == cell->needRender())
            cell->render(_pixels);
    }
}

void CRenderEngine::render()
{
    for (;;)
    {
        // rendering all units on memory
        renderUnits(cellUnits,   upw * uph,             pixels);
        renderUnits(horLines,    upw * (uph + 1),       pixels);
        renderUnits(vertLines,   (upw + 1) * uph,       pixels);
        renderUnits(squareUnits, (upw + 1) * (uph + 1), pixels);
        
        // selecting data from bitmap to working DC
        SelectObject(hWorkingDC, hBitmap);
        // copy data into main window DC
        BitBlt(hSourceDC, 0, 0, CSettingsManager::Instance().getScreenWidth(), CSettingsManager::Instance().getScreenHeight(), hWorkingDC, 0, 0, SRCCOPY);
        // repaint main window
        UpdateWindow(hSourceWnd);

        Sleep(CSettingsManager::Instance().getDelay());
    }
}

template<typename T>
void updateUnitState(T* _array, const long& _index, const bool& isCellActive)
{
    T* unit = &_array[_index];
    bool isUnitActive = unit->getState();
    if (isCellActive && isUnitActive)
        unit->changeVisibility(false);
    else
        unit->changeVisibility(true);
    unit->changeState();
}

void CRenderEngine::handleAction(actionType, const int& mouseX, const int& mouseY)
{
    // check line hit
    if (CSettingsManager::Instance().getBorderWidth() > (mouseX % (CSettingsManager::Instance().getCellWidth() + CSettingsManager::Instance().getBorderWidth()))
        || CSettingsManager::Instance().getBorderWidth() > (mouseY % (CSettingsManager::Instance().getCellHeight() + CSettingsManager::Instance().getBorderWidth()))
        || mouseX > (CSettingsManager::Instance().getCellWidth()* upw + CSettingsManager::Instance().getBorderWidth() * (upw + 1))
        || mouseY > (CSettingsManager::Instance().getCellHeight()* uph + CSettingsManager::Instance().getBorderWidth() * (uph + 1)))
        return;

    // detect hitted object
    int uX = (mouseX - CSettingsManager::Instance().getBorderWidth()) / (CSettingsManager::Instance().getCellWidth() + CSettingsManager::Instance().getBorderWidth());
    int uY = (mouseY - CSettingsManager::Instance().getBorderWidth()) / (CSettingsManager::Instance().getCellHeight() + CSettingsManager::Instance().getBorderWidth());

    CCellUnit* current = &cellUnits[uX + uY * upw];
    current->changeState();
    bool isCellActive = current->getState();

    // after hitting we need update states of all nearby units
    updateUnitState(horLines,    uX + uY * upw,                 isCellActive);  // top line
    updateUnitState(horLines,    uX + (uY + 1) * upw,           isCellActive);  // bottom line
    updateUnitState(vertLines,   uX + uY * (upw + 1),           isCellActive);  // left line
    updateUnitState(vertLines,   uX + 1 + uY * (upw + 1),       isCellActive);  // right line
    updateUnitState(squareUnits, uX + uY * (upw + 1),           isCellActive);  // top left square
    updateUnitState(squareUnits, uX + 1 + uY * (upw + 1),       isCellActive);  // top right square
    updateUnitState(squareUnits, uX + (uY + 1) * (upw + 1),     isCellActive);  // bottom left square
    updateUnitState(squareUnits, uX + 1 + (uY + 1) * (upw + 1), isCellActive);  // botton right square
}