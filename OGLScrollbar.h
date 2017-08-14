#include"OGLWidget.h"

#ifndef _OGL_SCROLLBAR_H_
#define _OGL_SCROLLBAR_H_

class OGLScrollbar : public OGLWidget
{
private:
    float mBarY;
    float mBarHeight;
    bool mBarDisable;
    bool mBarHover;
    bool mBarPressed;
    int mLastYClick;
    float mCurrentValue;
    float mMaxRangeValue;
    bool updateBarStatus(float targetYValue);
    bool inToRect(int x, int y, int xLeftRect, int yTopRect, int xRightRect, int yBottomRect);
    void drawBar();

public:
    /**/
    OGLScrollbar(OGLWidgetDescription description);
    /**/
    ~OGLScrollbar();

    /**/
    void setValue(float value);
    /**/
    void setMaxRange(float value);
    /**/
    float getValue();

    /**/
    void onEvent(OGLWidgetEvent& event);
};

#endif
