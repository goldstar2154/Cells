#pragma once

class CSettingsManager
{
public:
    static CSettingsManager& Instance()
    {
        static CSettingsManager single;
        return single;
    }

    CSettingsManager();

    long Read() const;
    void setScreenParam(long _screenWidth, long _screenHeight);

    long getScreenWidth() const;
    long getScreenHeight() const;

    int getCellWidth() const;
    int getCellHeight() const;
    int getBorderWidth() const;

    COLORREF getInactiveCellFill() const;
    COLORREF getInactiveCellBorder() const;
    COLORREF getActiveCellFill() const;
    COLORREF getActiveCellBorder() const;

    int getDelay() const;

private:
    CSettingsManager(CSettingsManager const&) = delete;
    CSettingsManager(CSettingsManager const&&) = delete;
    CSettingsManager& operator=(CSettingsManager const&) = delete;
    CSettingsManager& operator=(CSettingsManager const&&) = delete;

private:
    static const LPCWSTR configFileName;

    mutable long screenWidth;
    mutable long screenHeight;
    mutable int borderWidth;
    mutable int cellWidth;
    mutable int cellHeight;

    mutable COLORREF inactiveCellFill;
    mutable COLORREF inactiveCellBorder;
    mutable COLORREF activeCellFill;
    mutable COLORREF activeCellBorder;

    mutable int delay;
};

