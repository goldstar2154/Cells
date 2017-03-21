#pragma once

struct pixel;

class CCellUnit
{
public:
    CCellUnit();
    ~CCellUnit();

    void render(pixel* _pixels);
    void changeState();
    bool needRender() const;

public:
    int width;
    int height;
    int posX;
    int posY;

    bool isActive;
    bool isNeedRedraw;
    long animationState;

};

