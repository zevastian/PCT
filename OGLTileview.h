#include<memory>
#include"OGLWidget.h"
#include"OGLScrollbar.h"
#include"OGLLabel.h"

#ifndef _OGL_TILEVIEW_H_
#define _OGL_TILEVIEW_H_

class OGLTileview : public OGLWidget
{
private:
    std::shared_ptr<OGLScrollbar> mScrollbar;

public:
    /**/
    OGLTileview(OGLWidgetDescription description);
    /**/
    ~OGLTileview();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
