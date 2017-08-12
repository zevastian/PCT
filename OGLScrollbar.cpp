#include<epoxy/gl.h>
#include"OGLScrollbar.h"

OGLScrollbar::OGLScrollbar(OGLWidgetDescription description) : OGLWidget(description)
{

}

OGLScrollbar::~OGLScrollbar()
{

}

void OGLScrollbar::onEvent(OGLWidgetEvent& event)
{
    switch (event.type) {

    case OGL_WIDGET_MOUSE_MOVE:
        if (!mBarDisable) {
            //FALTA CALCULAR POSICION DE LA BARRA

            if (/*SOBRE LA BARRA*/) {
                if (!mBarHover) {
                    mBarHover = true;
                    if (!mBarPressed) {
                        //NOTIFICO DRAW
                    }
                }
            } else {
                if (mBarHover) {
                    mBarHover = false;
                    if (!mBarPressed) {
                        //NOTIFICO DRAW
                    }
                }
            }
        }
        break;

    case OGL_WIDGET_DRAW:
        //ESTO LUEGO VA A TENER UNA IMPLEMENTACION INDEPENDIENTE
        //DE LA PLATAFORMA
        glBegin(GL_QUADS);
        glColor3f(0.887f, 0.878f, 0.867f);

        glVertex2f(OGLWidget::mXLeft, OGLWidget::mYTop);
        glVertex2f(OGLWidget::mXRight, OGLWidget::mYTop);
        glVertex2f(OGLWidget::mXRight, OGLWidget::mYBottom);
        glVertex2f(OGLWidget::mXLeft, OGLWidget::mYBottom);

        if (!mBarDisable) {
            if (!mBarHover && !mBarPressed) {
                glColor3f(0.914f, 0.329f, 0.125f);
            } else {
                glColor3f(0.929f, 0.463f, 0.302f);
            }
            glVertex2f(OGLWidget::mXLeft, OGLWidget::mYTop + mBarY);
            glVertex2f(OGLWidget::mXRight, OGLWidget::mYTop + mBarY);
            glVertex2f(OGLWidget::mXRight, OGLWidget::mYTop + mBarY + mBarHeight);
            glVertex2f(OGLWidget::mXLeft, OGLWidget::mYTop + mBarY + mBarHeight);
        }

        glEnd();
        break;

    case OGL_WIDGET_MOUSE_CLICK_DOWN:
        if (!mBarDisable) {
            if (/*SOBRE EL WIDGET*/) {
                //FALTA CALCULAR POSICION DE LA BARRA
            }
        }
        break;

    case OGL_WIDGET_MOUSE_CLICK_UP:
        if (!mBarDisable) {
            if (mBarPressed) {
                mBarPressed = false;
                if (!mBarHover) {
                    //NOTIFICO DRAW
                }
            }
        }
        break;

    case OGL_WIDGET_MOUSE_WHEEL:
        if (!mBarPressed && !mBarDisable) {
            //FALTA CALCULAR POSICION DE LA BARRA
        }
        break;

    case OGL_WIDGET_MOUSE_LEAVE:
        if (mBarHover && !mBarPressed) {
            mBarHover = false;
            //NOTIFICO DRAW
        }
        break;

    case OGL_WIDGET_FOCUS_RELEASE:
        if (mBarPressed || mBarHover) {
            mBarPressed = false;
            mBarHover = false;
            //NOTIFICO DRAW
        }
        break;

    case OGL_WIDGET_SIZE:
        //FALTA CALCULAR POSICION DE LA BARRA
        break;
    }
}
