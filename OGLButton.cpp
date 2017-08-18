#include<epoxy/gl.h>
#include"OGLButton.h"
#include"OGLUtils.h"

//SE PODRIA HACER UN WIDGET CLICKLEABLE GENERICO
//QUE COMPARTEN TODOS LOS TIPOS DE BOTONES
OGLButton::OGLButton(OGLButtonDescription description) : OGLWidget(description.widget)
{
    //FALTA CHEQUEAR CAMPOS
    onClick = description.button.onClick;
}

OGLButton::~OGLButton()
{

}

int OGLButton::onEvent(OGLWidgetEvent event)
{
    int ret = OGL_WIDGET_RET_NONE;

    switch (event.type) {
    case OGL_WIDGET_MOUSE_MOVE:
        if (utils::inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                            OGLWidget::mXLeft, OGLWidget::mYTop,
                            OGLWidget::mXRight, OGLWidget::mYBottom)) {

            if (!mHover) {
                mHover = true;
                ret |= OGL_WIDGET_RET_DRAW;
            }
        } else {
            if (mHover) {
                mHover = false;
                ret |= OGL_WIDGET_RET_DRAW;
            }
        }
        break;

    case OGL_WIDGET_MOUSE_LEAVE:
    case OGL_WIDGET_FOCUS_RELEASE:
        if (mHover) {
            mHover = false;
            ret |= OGL_WIDGET_RET_DRAW;
        }
        break;

    case OGL_WIDGET_MOUSE_CLICK_DOWN:
        if (utils::inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                            OGLWidget::mXLeft, OGLWidget::mYTop,
                            OGLWidget::mXRight, OGLWidget::mYBottom)) {

            onClick();
            mHover = true;
            ret |= OGL_WIDGET_RET_DRAW;
        }
        break;

    case OGL_WIDGET_DRAW:
        //ESTO LUEGO TENDRA UNA IMPLEMENTACION
        //INDEPENDIENTE DE LA PLATAFORMA
        if (!mHover) {
            glColor4f(0.914f, 0.329f, 0.125f, 1.0f);
        } else {
            glColor4f(0.929f, 0.463f, 0.302f, 1.0f);
        }

        glBegin(GL_QUADS);
        glVertex2f(OGLWidget::mXLeft, OGLWidget::mYTop);
        glVertex2f(OGLWidget::mXRight, OGLWidget::mYTop);
        glVertex2f(OGLWidget::mXRight, OGLWidget::mYBottom);
        glVertex2f(OGLWidget::mXLeft, OGLWidget::mYBottom);
        glEnd();
        break;

    case OGL_WIDGET_SIZE:
        OGLWidget::onEvent(event);
        //NO ES NECESARIO SOLICITAR UN DRAW
        break;

    case OGL_WIDGET_REQUEST_EVENT_MASK:
        ret = OGLWidget::onEvent(event) | OGL_WIDGET_MOUSE_MOVE |
              OGL_WIDGET_MOUSE_LEAVE | OGL_WIDGET_FOCUS_RELEASE |
              OGL_WIDGET_MOUSE_CLICK_DOWN | OGL_WIDGET_DRAW |
              OGL_WIDGET_SIZE;
        break;
    }

    return ret;
}
