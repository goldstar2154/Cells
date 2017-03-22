#include "stdafx.h"
#include "SettingsManager.h"

const LPCWSTR CSettingsManager::configFileName = L"config.ini";

const LPWSTR defaultInactiveCellFill = L"190, 190, 190";
const LPWSTR defaultInactiveCellBorder = L"120, 120, 120";
const LPWSTR defaultActiveCellFill = L"255, 255, 255";
const LPWSTR defaultActiveCellBorder = L"0, 0, 0";

const int defaultScreenWidth = 0;
const int defaultScreenHeight = 0;
const int defaultCellWidth = 25;
const int defaultCellHeight = 25;
const int defaultBorderWidth = 2;
const int defaultDelay = 10;

CSettingsManager::CSettingsManager()
    : screenWidth(0)
    , screenHeight(0)
    , cellWidth(0)
    , cellHeight(0)
    , borderWidth(0)
    , inactiveCellFill(RGB(0, 0, 0))
    , inactiveCellBorder(RGB(0, 0, 0))
    , activeCellFill(RGB(0, 0, 0))
    , activeCellBorder(RGB(0, 0, 0))
    , delay(0)
{
}

 long wcharToColor(LPWSTR source, COLORREF& color)
{
    int R = 0;
    int G = 0;
    int B = 0;

    LPWSTR delim = L",";
    LPWSTR context;

    LPWSTR token = wcstok_s(source, delim, &context);

    size_t i = 0;
    while (token != NULL)
    {
        switch (i)
        {
        case 0: R = _ttoi(token); break;
        case 1: G = _ttoi(token); break;
        case 2: B = _ttoi(token); break;
        default: break;
        }

        ++i;
        token = wcstok_s(NULL, delim, &context);
    }

    color = RGB(R, G, B);

    return 0;
}

long CSettingsManager::Read() const
{
    WCHAR configFilePath[MAX_PATH + 1] = { 0 };


    GetModuleFileName(0, configFilePath, MAX_PATH);
    
    for (size_t i = wcslen(configFilePath) - 1; i > 2; i--)
    {
        if (configFilePath[i - 1] == '\\')
        {
            configFilePath[i] = '\0';
            break;
        }
    }

    wcscat_s(configFilePath, MAX_PATH, L"//");
    wcscat_s(configFilePath, MAX_PATH, configFileName);

    borderWidth = GetPrivateProfileInt(L"Border",    L"Width",  defaultBorderWidth, configFilePath);
    cellWidth   = GetPrivateProfileInt(L"Cell",      L"Width",  defaultCellWidth,   configFilePath);
    cellHeight  = GetPrivateProfileInt(L"Cell",      L"Height", defaultCellHeight,  configFilePath);
    delay       = GetPrivateProfileInt(L"Animation", L"Delay",  defaultDelay,       configFilePath);

    WCHAR tmp[256] = { 0 };
    GetPrivateProfileString(L"Cell", L"InactiveCellFill", defaultInactiveCellFill, tmp, 14, configFilePath);
    wcharToColor(tmp, inactiveCellFill);
    
    memset(tmp, 0, sizeof(tmp));
    GetPrivateProfileString(L"Cell", L"InactiveCellBorder", defaultInactiveCellBorder, tmp, 14, configFilePath);
    wcharToColor(tmp, inactiveCellBorder);
   
    memset(tmp, 0, sizeof(tmp));
    GetPrivateProfileString(L"Cell", L"ActiveCellFill", defaultActiveCellFill, tmp, 14, configFilePath);
    wcharToColor(tmp, activeCellFill);
    
    memset(tmp, 0, sizeof(tmp));
    GetPrivateProfileString(L"Cell", L"ActiveCellBorder", defaultActiveCellBorder, tmp, 14, configFilePath);
    wcharToColor(tmp, activeCellBorder);

    return 0;
}

void CSettingsManager::setScreenParam(const long& _screenWidth, const long& _screenHeight)
{
    screenWidth = _screenWidth;
    screenHeight = _screenHeight;
}

long CSettingsManager::getScreenWidth() const
{
    return screenWidth;
}

long CSettingsManager::getScreenHeight() const
{
    return screenHeight;
}

int CSettingsManager::getCellWidth() const
{
    return cellWidth;
}

int CSettingsManager::getCellHeight() const
{
    return cellHeight;
}

int CSettingsManager::getBorderWidth() const
{
    return borderWidth;
}

COLORREF CSettingsManager::getInactiveCellFill() const
{
    return inactiveCellFill;
}

COLORREF CSettingsManager::getInactiveCellBorder() const
{
    return inactiveCellBorder;
}

COLORREF CSettingsManager::getActiveCellFill() const
{
    return activeCellFill;
}

COLORREF CSettingsManager::getActiveCellBorder() const
{
    return activeCellBorder;
}

int CSettingsManager::getDelay() const
{
    return delay;
}
