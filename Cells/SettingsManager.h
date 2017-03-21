// Incapsulates work with settings file
#pragma once

#include <string>

class CSettingsManager
{
public:
    static const CSettingsManager& Instance()
    {
        static CSettingsManager single;
        return single;
    }

    long Read() const;

    int getCellWidth() const;
    int getCellHeight() const;
    COLORREF getInactiveCellFill() const;
    COLORREF getInactiveCellBorder() const;
    COLORREF getActiveCellFill() const;
    COLORREF getActiveCellBorder() const;

private:
    CSettingsManager();
    ~CSettingsManager();

private:
    static const LPCWSTR configFileName;
    mutable int cellWidth;
    mutable int cellHeight;

    mutable COLORREF inactiveCellFill;
    mutable COLORREF inactiveCellBorder;
    mutable COLORREF activeCellFill;
    mutable COLORREF activeCellBorder;

};

