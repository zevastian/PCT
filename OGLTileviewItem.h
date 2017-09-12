#include"OGLIWidget.h"

#ifndef _OGL_TILEVIEWITEM_H_
#define _OGL_TILEVIEWITEM_H_

class OGLTileviewItem : public OGLIWidget
{
private:
    float mX;
    float mY;
    float mWidth;
    float mHeight;
    bool mFocused;

public:
    /**/
    OGLTileviewItem();
    /**/
    ~OGLTileviewItem();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
