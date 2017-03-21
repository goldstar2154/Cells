#pragma once

struct pixel;

enum class animationStatus
{
    stop,
    playForward,
    playBack,
    firstDraw
};

class CBaseUnit
{
public:
    CBaseUnit()
        : width(0)
        , height(0)
        , x(0)
        , y(0)
        , isActive(false)
        , isNeedRedraw(true)
        , animation(animationStatus::firstDraw)
    {

    }

    virtual ~CBaseUnit()
    {

    }

    virtual void render(pixel* _pixels) = 0;

    void setSizes(const int& _width, const int& _height)
    {
        width = _width;
        height = _height;
    }

    void setPos(const int& _x, const int& _y)
    {
        x = _x;
        y = _y;
    }

    virtual void changeState()
    {
        isActive = !isActive;
        isNeedRedraw = true;
    }

    bool getState() const
    {
        return isActive;
    }

    bool needRender()
    {
        return isNeedRedraw;
    }

protected:
    int width;
    int height;
    int x;
    int y;

    bool isActive;
    bool isNeedRedraw;

    animationStatus animation;
};

