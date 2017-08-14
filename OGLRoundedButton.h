#include"OGLWidget.h"

#ifndef _OGL_ROUNDED_BUTTON_H_
#define _OGL_ROUNDED_BUTTON_H_

typedef void (*OGLOnClickCallback)(void);

struct OGLRoundedButtonDescription {
    OGLWidgetDescription widget;
    struct {
        OGLOnClickCallback onClick;
        float radius;
    } button;
    OGLRoundedButtonDescription()
    {
        button.onClick = nullptr;
        button.radius = 0.0f;
    }
};

class OGLRoundedButton : public OGLWidget
{
private:
    bool mHover;
    float mRadius;
    OGLOnClickCallback onClick;
    bool overButton(int x, int y);
    void drawArc(float x, float y, float offset, float arc, int steps);
    void draw();

public:
    /**/
    OGLRoundedButton(OGLRoundedButtonDescription description);
    /**/
    ~OGLRoundedButton();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
