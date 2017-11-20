#include"OGLIWidget.h"

#ifndef _OGL_WIDGET_H_
#define _OGL_WIDGET_H_

#define OGL_WIDGET_RET_NONE 0x0000
#define OGL_WIDGET_RET_DRAW 0x0001
#define OGL_WIDGET_RET_FOCUS_GET 0x002
#define OGL_WIDGET_RET_FOCUS_RELEASE 0x004
#define OGL_WIDGET_RET_SCROLL_CHANGE_VALUE 0x008

enum OGLWidgetXFlag {
    OGL_XFLAG_NONE,
    OGL_RELATIVE_LEFT,
    OGL_RELATIVE_RIGHT,
    OGL_ALIGN_LEFT,
    OGL_ALIGN_RIGHT,
    OGL_ALIGN_CENTER_X
};

enum OGLWidgetYFlag {
    OGL_YFLAG_NONE,
    OGL_RELATIVE_TOP,
    OGL_RELATIVE_BOTTOM,
    OGL_ALIGN_TOP,
    OGL_ALIGN_BOTTOM,
    OGL_ALIGN_CENTER_Y
};

enum OGLWidgetDimensionFlag {
    OGL_DIMFLAG_NONE,
    OGL_PX,
    OGL_PERCENT
};

struct OGLWidgetDescription {
    struct {
        float value;
        OGLWidgetXFlag flag;
    } x;
    struct {
        float value;
        OGLWidgetYFlag flag;
    } y;
    struct {
        float value;
        OGLWidgetDimensionFlag flag;
    } width;
    struct {
        float value;
        OGLWidgetDimensionFlag flag;
    } height;
    OGLWidgetDescription()
    {
        x.flag = OGL_XFLAG_NONE;
        y.flag = OGL_YFLAG_NONE;
        width.flag = OGL_DIMFLAG_NONE;
        height.flag = OGL_DIMFLAG_NONE;
    }
};

class OGLWidget : public OGLIWidget
{
private:
    float mXParent;
    float mYParent;
    float mXLeft;
    float mXRight;
    float mYTop;
    float mYBottom;
    OGLWidgetDescription mDescription;

protected:
    float getXLeft();
    float getXRight();
    float getYTop();
    float getYBottom();

public:
    /**/
    OGLWidget(OGLWidgetDescription description);
    /**/
    ~OGLWidget();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
