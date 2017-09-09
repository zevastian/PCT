#include<epoxy/gl.h>
#include"OGLWidget.h"
#include"OGLTileviewItem.h"
#include"OGLUtils.h"

OGLTileviewItem::OGLTileviewItem()
{

}

OGLTileviewItem::~OGLTileviewItem()
{

}

int OGLTileviewItem::onEvent(OGLWidgetEvent event)
{
    int ret = OGL_WIDGET_RET_NONE;
    switch (event.type) {

    case OGL_WIDGET_MOUSE_MOVE:
        if (utils::inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                            mXLeft, mYTop, mXRight, mYBottom)) {
            mFocused = true;
            ret = OGL_WIDGET_RET_FOCUS_GET;
        }
        break;

    case OGL_WIDGET_MOVE:
        mXLeft = OGL_WIDGET_MOVE_GET_X(event);
        mYTop = OGL_WIDGET_MOVE_GET_Y(event);
        break;

    case OGL_WIDGET_SIZE:
        mXRight = mXLeft + OGL_WIDGET_SIZE_GET_WIDTH(event);
        mYBottom = mYTop + OGL_WIDGET_SIZE_GET_HEIGHT(event);
        break;

    case OGL_WIDGET_FOCUS_RELEASE:
        mFocused = false;
        break;

    case OGL_WIDGET_DRAW:
        //SOLO PARA PROBAR
        if (mFocused) {
            glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        } else {
            glColor4f(0.667f, 0.667f, 0.667f, 1.0f);
        }
        glBegin(GL_QUADS);
        glVertex2f(mXLeft, mYTop);
        glVertex2f(mXRight, mYTop);
        glVertex2f(mXRight, mYBottom);
        glVertex2f(mXLeft, mYBottom);
        glEnd();
        break;
    }

    return ret;
}
