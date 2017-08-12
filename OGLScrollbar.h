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

public:
    /**/
    OGLScrollbar(OGLWidgetDescription description);
    /**/
    ~OGLScrollbar();

    /**/
    void onEvent(OGLWidgetEvent& event);
};

#endif
