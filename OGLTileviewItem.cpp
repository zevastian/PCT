#include<epoxy/gl.h>
#include"OGLWidget.h"
#include"OGLTileviewItem.h"
#include"OGLUtils.h"

OGLTileviewItem::OGLTileviewItem()
{
    mFocused = false;
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
                            mX, mY, mX + mWidth, mY + mHeight)) {

            ret |= OGL_WIDGET_RET_FOCUS_GET | OGL_WIDGET_RET_DRAW;
            mFocused = true;
        } else {
            mFocused = false;
        }
        break;

    case OGL_WIDGET_MOVE:
        mX = OGL_WIDGET_MOVE_GET_X(event);
        mY = OGL_WIDGET_MOVE_GET_Y(event);
        break;

    case OGL_WIDGET_SIZE:
        mWidth = OGL_WIDGET_SIZE_GET_WIDTH(event);
        mHeight = OGL_WIDGET_SIZE_GET_HEIGHT(event);
        break;

    case OGL_WIDGET_FOCUS_RELEASE:
        mFocused = false;
        ret |= OGL_WIDGET_RET_DRAW;
        break;

    case OGL_WIDGET_DRAW:
        //SOLO PARA PROBAR
        if (mFocused) {
            glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        } else {
            glColor4f(0.667f, 0.667f, 0.667f, 1.0f);
        }
        glBegin(GL_QUADS);
        glVertex2f(mX, mY);
        glVertex2f(mX + mWidth, mY);
        glVertex2f(mX + mWidth, mY + mHeight);
        glVertex2f(mX, mY + mHeight);
        glEnd();
        break;
    }

    return ret;
}
