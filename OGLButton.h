#include"OGLWidget.h"

#ifndef _OGL_BUTTON_H_
#define _OGL_BUTTON_H_

typedef void (*OGLOnClickCallback)(void);

struct OGLButtonDescription {
    OGLWidgetDescription widget;
    struct {
        OGLOnClickCallback onClick;
    } button;
    OGLButtonDescription()
    {
        button.onClick = nullptr;
    }
};

class OGLButton : public OGLWidget
{
private:
    bool mHover;
    OGLOnClickCallback onClick;

public:
    /**/
    OGLButton(OGLButtonDescription description);
    /**/
    ~OGLButton();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
