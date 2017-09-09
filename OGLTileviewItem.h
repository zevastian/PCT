#include"OGLIWidget.h"

#ifndef _OGL_TILEVIEWITEM_H_
#define _OGL_TILEVIEWITEM_H_

class OGLTileviewItem : public OGLIWidget
{
private:
    float mXLeft;
    float mXRight;
    float mYTop;
    float mYBottom;
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
