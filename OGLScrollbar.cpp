#include<epoxy/gl.h>
#include"OGLScrollbar.h"
#include"OGLUtils.h"

OGLScrollbar::OGLScrollbar(OGLWidgetDescription description) : OGLWidget(description)
{
    //FALTA INICIALIZAR
    //LAS VARIABLES
}

OGLScrollbar::~OGLScrollbar()
{

}

bool OGLScrollbar::updateBarStatus(float targetYValue, bool updateCurrentValue)
{
    float widgetHeight = OGLWidget::getYBottom() - OGLWidget::getYTop();
    float maxValue = widgetHeight - mBarHeight;

    if (targetYValue > maxValue) {
        targetYValue = maxValue;
    } else if (targetYValue < 0.0f) {
        targetYValue = 0.0f;
    }

    if (mBarY != targetYValue) {
        mBarY = targetYValue;
        if (updateCurrentValue) {
            mCurrentValue = ((mMaxRangeValue - widgetHeight)*mBarY)/(widgetHeight - mBarHeight);
        }
        return true;
    }
    return false;
}

void OGLScrollbar::drawBar()
{
    //ESTO LUEGO VA A TENER UNA IMPLEMENTACION INDEPENDIENTE
    //DE LA PLATAFORMA
    glBegin(GL_QUADS);
    glColor3f(0.887f, 0.878f, 0.867f);

    glVertex2f(OGLWidget::getXLeft(), OGLWidget::getYTop());
    glVertex2f(OGLWidget::getXRight(), OGLWidget::getYTop());
    glVertex2f(OGLWidget::getXRight(), OGLWidget::getYBottom());
    glVertex2f(OGLWidget::getXLeft(), OGLWidget::getYBottom());

    if (!mBarDisable) {
        if (!mBarHover && !mBarPressed) {
            glColor3f(0.914f, 0.329f, 0.125f);
        } else {
            glColor3f(0.929f, 0.463f, 0.302f);
        }
        glVertex2f(OGLWidget::getXLeft(), OGLWidget::getYTop() + mBarY);
        glVertex2f(OGLWidget::getXRight(), OGLWidget::getYTop() + mBarY);
        glVertex2f(OGLWidget::getXRight(), OGLWidget::getYTop() + mBarY + mBarHeight);
        glVertex2f(OGLWidget::getXLeft(), OGLWidget::getYTop() + mBarY + mBarHeight);
    }

    glEnd();
}

void OGLScrollbar::setValue(float value)
{
    if (value >= 0.0f && value <= mMaxRangeValue) {
        mCurrentValue = value;
    }
}

float OGLScrollbar::getValue()
{
    return !mBarDisable ? mCurrentValue : 0.0f;
}

void OGLScrollbar::setMaxRangeValue(float value)
{
    if (value >= 0.0f) {
        mMaxRangeValue = value;
    }
}

float OGLScrollbar::getMaxRangeValue()
{
    return mMaxRangeValue;
}

int OGLScrollbar::onEvent(OGLWidgetEvent event)
{
    int ret = OGL_WIDGET_RET_NONE;

    switch (event.type) {
    case OGL_WIDGET_MOUSE_MOVE:
        if (!mBarDisable) {
            if (mBarPressed) {
                if (updateBarStatus(OGL_WIDGET_MOUSE_GET_Y(event) - OGLWidget::getYTop() - mLastYClick, true)) {
                    ret |= OGL_WIDGET_RET_DRAW | OGL_WIDGET_RET_SCROLL_CHANGE_VALUE;
                }
            }
            if (utils::inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                                OGLWidget::getXLeft(), OGLWidget::getYTop() + mBarY,
                                OGLWidget::getXRight(), OGLWidget::getYTop() + mBarY + mBarHeight)) {

                if (!mBarHover) {
                    mBarHover = true;
                    if (!mBarPressed) {
                        ret |= OGL_WIDGET_RET_DRAW;
                    }
                }
            } else {
                if (mBarHover) {
                    mBarHover = false;
                    if (!mBarPressed) {
                        ret |= OGL_WIDGET_RET_DRAW;
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
            float barWidth = OGLWidget::getXRight() - OGLWidget::getXLeft();
            float barXCenter = OGLWidget::getXLeft() + 0.5f*barWidth;
            if (utils::inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                                barXCenter - SCROLL_WIDTH_MULT*barWidth, OGLWidget::getYTop(),
                                barXCenter + SCROLL_WIDTH_MULT*barWidth, OGLWidget::getYBottom())) {

                mBarPressed = true;
                ret |= OGL_WIDGET_RET_FOCUS_GET;
                if (!utils::inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                                     barXCenter - SCROLL_WIDTH_MULT*barWidth, OGLWidget::getYTop() + mBarY,
                                     barXCenter + SCROLL_WIDTH_MULT*barWidth, OGLWidget::getYTop() + mBarY + mBarHeight)) {

                    if (updateBarStatus(OGL_WIDGET_MOUSE_GET_Y(event) - OGLWidget::getYTop() - 0.5f*mBarHeight, true)) {
                        mBarHover = true;
                        ret |= OGL_WIDGET_RET_DRAW | OGL_WIDGET_RET_SCROLL_CHANGE_VALUE;
                    }
                }
                mLastYClick = OGL_WIDGET_MOUSE_GET_Y(event) - OGLWidget::getYTop() - mBarY;
            }
        }
        break;

    case OGL_WIDGET_MOUSE_CLICK_UP:
        if (!mBarDisable) {
            if (mBarPressed) {
                ret |= OGL_WIDGET_RET_FOCUS_RELEASE;
                mBarPressed = false;
                if (!mBarHover) {
                    ret |= OGL_WIDGET_RET_DRAW;
                }
            }
        }
        break;

    case OGL_WIDGET_MOUSE_WHEEL:
        if (!mBarPressed && !mBarDisable) {
            float maxOffset = ((OGLWidget::getYBottom() - OGLWidget::getYTop()) - mBarHeight)/mMaxRangeValue;
            if (updateBarStatus(mBarY - (OGL_WIDGET_MOUSE_GET_DELTA(event)*SCROLL_ADVANCE*maxOffset), true)) {
                ret |= OGL_WIDGET_RET_DRAW | OGL_WIDGET_RET_SCROLL_CHANGE_VALUE;
            }
        }
        break;

    case OGL_WIDGET_MOUSE_LEAVE:
        if (mBarHover && !mBarPressed) {
            mBarHover = false;
            ret |= OGL_WIDGET_RET_DRAW;
        }
        break;

    case OGL_WIDGET_FOCUS_RELEASE:
        if (mBarPressed || mBarHover) {
            mBarPressed = false;
            mBarHover = false;
            ret |= OGL_WIDGET_RET_FOCUS_RELEASE;
            ret |= OGL_WIDGET_RET_DRAW;
        }
        break;

    case OGL_WIDGET_REQUEST_EVENT_MASK:
        ret = OGLWidget::onEvent(event) | OGL_WIDGET_MOUSE_MOVE |
              OGL_WIDGET_ANIMATE | OGL_WIDGET_DRAW | OGL_WIDGET_MOUSE_CLICK_DOWN |
              OGL_WIDGET_MOUSE_CLICK_UP | OGL_WIDGET_MOUSE_WHEEL |
              OGL_WIDGET_MOUSE_LEAVE | OGL_WIDGET_FOCUS_RELEASE |
              OGL_WIDGET_SIZE | OGL_WIDGET_MOVE;
        break;

    case OGL_WIDGET_MOVE:
        OGLWidget::onEvent(event);
        break;

    case OGL_WIDGET_SIZE:
        OGLWidget::onEvent(event);
        //EN UN EVENTO SIZE NO ES NECESARIO NOTIFICAR
        //QUE SE NECECITA ACTUALIZAR EL CONTENIDO
        if (!mBarPressed) {
            mBarHover = false;
        }

        float widgetHeight = OGLWidget::getYBottom() - OGLWidget::getYTop();
        mBarHeight = (mMaxRangeValue > 0.0f) ? (widgetHeight*widgetHeight)/mMaxRangeValue : widgetHeight;
        if (mBarHeight >= widgetHeight) {
            mBarHeight = widgetHeight;
            mBarDisable = true;
        } else {
            mBarDisable = false;
            updateBarStatus((mCurrentValue*(widgetHeight - mBarHeight))/(mMaxRangeValue - widgetHeight), false);
        }
        //HAY CASOS EN EL QUE EN REALIDAD NO CAMBIA
        ret |= OGL_WIDGET_RET_SCROLL_CHANGE_VALUE;
        break;
    }

    return ret;
}
