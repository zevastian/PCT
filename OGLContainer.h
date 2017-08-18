#include<vector>
#include<memory>
#include"OGLIWidget.h"
#include"OGLWidget.h"

#ifndef _OGL_CONTAINER_H_
#define _OGL_CONTAINER_H_

class OGLContainer : public OGLWidget
{
private:
    struct Children {
        std::shared_ptr<OGLIWidget> widget;
        int eventMask;
    };
    std::vector<Children> mChildrens;

public:
    /**/
    OGLContainer(OGLWidgetDescription description);
    /**/
    ~OGLContainer();

    /**/
    int onEvent(OGLWidgetEvent event);

    /**/
    void add(std::shared_ptr<OGLIWidget> widget);
};

#endif
