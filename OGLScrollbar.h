#include"OGLWidget.h"

#ifndef _OGL_SCROLLBAR_H_
#define _OGL_SCROLLBAR_H_

#define SCROLL_ADVANCE 64.0f
#define SCROLL_WIDTH_MULT 1.5f

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
    bool updateBarStatus(float targetYValue, bool updateCurrentValue);
    void drawBar();

public:
    /**/
    OGLScrollbar(OGLWidgetDescription description);
    /**/
    ~OGLScrollbar();

    /**/
    void setValue(float value);
    /**/
    float getValue();
    /**/
    void setMaxRangeValue(float value);
    /**/
    float getMaxRangeValue();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
