#include<epoxy/gl.h>
#include"OGLScrollbar.h"

OGLScrollbar::OGLScrollbar(OGLWidgetDescription description) : OGLWidget(description)
{

}

OGLScrollbar::~OGLScrollbar()
{

}

bool OGLScrollbar::changeBarPosition(float targetYValue)
{
    float maxValue = (OGLWidget::mYBottom - OGLWidget::mYTop) - mBarHeight;
    if (targetYValue > maxValue) {
        targetYValue = maxValue;
    } else if (targetYValue < 0.0f) {
        targetYValue = 0.0f;
    }
    if (mBarY != targetYValue) {
        mBarY = targetYValue;
        return true;
    }
    return false;
}

void OGLScrollbar::updateCurrentValue()
{
    float widgetHeight = OGLWidget::mYBottom - OGLWidget::mYTop;
    mCurrentValue = ((mMaxRangeValue - widgetHeight)*mBarY)/(widgetHeight - mBarHeight);
}

bool OGLScrollbar::inToRect(int x, int y, int xLeftRect, int yTopRect, int xRightRect, int yBottomRect)
{
    return (x > xLeftRect) && (x < xRightRect) &&
           (y > yTopRect) && (y < yBottomRect);
}

void OGLScrollbar::drawBar()
{
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
}

void OGLScrollbar::setValue(float value)
{
    if (value >= 0.0f && value <= mMaxRangeValue) {
        mCurrentValue = value;
    }
}

void OGLScrollbar::setMaxRange(float value)
{
    if (value >= 0.0f) {
        mMaxRangeValue = value;
    }
}

float OGLScrollbar::getValue()
{
    return mCurrentValue;
}

void OGLScrollbar::onEvent(OGLWidgetEvent& event)
{
    switch (event.type) {

    case OGL_WIDGET_MOUSE_MOVE:
        if (!mBarDisable) {
            if (mBarPressed) {
                if (changeBarPosition(OGL_WIDGET_MOUSE_GET_Y(event) - OGLWidget::mYTop - mLastYClick)) {
                    updateCurrentValue();
                    //NOTIFICO DRAW
                }
            }
            if (inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                         OGLWidget::mXLeft, OGLWidget::mYTop + mBarY,
                         OGLWidget::mXRight, OGLWidget::mYTop + mBarY + mBarHeight)) {

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

    case OGL_WIDGET_ANIMATE:
        break;

    case OGL_WIDGET_DRAW:
        drawBar();
        break;

    case OGL_WIDGET_MOUSE_CLICK_DOWN:
        if (!mBarDisable) {
            float barWidth = OGLWidget::mXRight - OGLWidget::mXLeft;
            float barXCenter = OGLWidget::mXLeft + 0.5f*barWidth;
            if (inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                         barXCenter - 1.5f*barWidth, OGLWidget::mYTop,
                         barXCenter + 1.5f*barWidth, OGLWidget::mYBottom)) {

                //HAY UN CAMBIO DE FOCO QUE TENGO QUE NOTIFICAR
                mBarPressed = true;
                if (!inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                              barXCenter - 1.5f*barWidth, OGLWidget::mYTop + mBarY,
                              barXCenter + 1.5f*barWidth, OGLWidget::mYTop + mBarY + mBarHeight)) {

                    if (changeBarPosition(OGL_WIDGET_MOUSE_GET_Y(event) - OGLWidget::mYTop - 0.5f*mBarHeight)) {
                        mBarHover = true;
                        updateCurrentValue();
                        //NOTIFICO DRAW
                    }
                }
                mLastYClick = OGL_WIDGET_MOUSE_GET_Y(event) - OGLWidget::mYTop - mBarY;
            }
        }
        break;

    case OGL_WIDGET_MOUSE_CLICK_UP:
        if (!mBarDisable) {
            if (mBarPressed) {
                //HAY UN CAMBIO DE FOCO QUE TENGO QUE NOTIFICAR
                mBarPressed = false;
                if (!mBarHover) {
                    //NOTIFICO DRAW
                }
            }
        }
        break;

    case OGL_WIDGET_MOUSE_WHEEL:
        if (!mBarPressed && !mBarDisable) {
            //CONVIENE PONER UN SCROLL_ADVANCE PARA INDICAR LAS UNIDADES DE SALTO
            float maxOffset = ((OGLWidget::mYBottom - OGLWidget::mYTop) - mBarHeight)/mMaxRangeValue;
            if (changeBarPosition(mBarY - (OGL_WIDGET_MOUSE_GET_DELTA(event)*64.0f*maxOffset))) {
                updateCurrentValue();
                //NOTIFICO DRAW
            }
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
            //HAY UN CAMBIO DE FOCO QUE TENGO QUE NOTIFICAR
            //NOTIFICO DRAW
        }
        break;

    case OGL_WIDGET_SIZE:
        OGLWidget::onEvent(event);

        mBarHover = false;
        if (mBarPressed) {
            mBarPressed = false;
            //HAY UN CAMBIO DE FOCO QUE TENGO QUE NOTIFICAR
        }

        float widgetHeight = OGLWidget::mYBottom - OGLWidget::mYTop;
        mBarHeight = (widgetHeight*widgetHeight)/mMaxRangeValue;
        if (mBarHeight >= widgetHeight) {
            mBarHeight = widgetHeight;
            mBarDisable = true;
            mCurrentValue = 0.0f;
        } else {
            if (mBarDisable) {
                mBarDisable = false;
                mCurrentValue = 0.0f;
            }
            changeBarPosition((mCurrentValue*(widgetHeight - mBarHeight))/(mMaxRangeValue - widgetHeight));
        }

        //NOTIFICO DRAW
        break;
    }
}
