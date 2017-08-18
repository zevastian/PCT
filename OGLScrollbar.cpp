#include<epoxy/gl.h>
#include"OGLScrollbar.h"
#include"OGLUtils.h"

OGLScrollbar::OGLScrollbar(OGLWidgetDescription description) : OGLWidget(description)
{
    //NADA
}

OGLScrollbar::~OGLScrollbar()
{

}

bool OGLScrollbar::updateBarStatus(float targetYValue)
{
    float widgetHeight = OGLWidget::mYBottom - OGLWidget::mYTop;
    float maxValue = widgetHeight - mBarHeight;

    if (targetYValue > maxValue) {
        targetYValue = maxValue;
    } else if (targetYValue < 0.0f) {
        targetYValue = 0.0f;
    }

    if (mBarY != targetYValue) {
        mBarY = targetYValue;
        mCurrentValue = ((mMaxRangeValue - widgetHeight)*mBarY)/(widgetHeight - mBarHeight);
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

int OGLScrollbar::onEvent(OGLWidgetEvent event)
{
    int ret = OGL_WIDGET_RET_NONE;

    switch (event.type) {
    case OGL_WIDGET_MOUSE_MOVE:
        if (!mBarDisable) {
            if (mBarPressed) {
                if (updateBarStatus(OGL_WIDGET_MOUSE_GET_Y(event) - OGLWidget::mYTop - mLastYClick)) {
                    ret |= OGL_WIDGET_RET_DRAW;
                }
            }
            if (utils::inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                                OGLWidget::mXLeft, OGLWidget::mYTop + mBarY,
                                OGLWidget::mXRight, OGLWidget::mYTop + mBarY + mBarHeight)) {

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
            float barWidth = OGLWidget::mXRight - OGLWidget::mXLeft;
            float barXCenter = OGLWidget::mXLeft + 0.5f*barWidth;
            if (utils::inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                                barXCenter - SCROLL_WIDTH_MULT*barWidth, OGLWidget::mYTop,
                                barXCenter + SCROLL_WIDTH_MULT*barWidth, OGLWidget::mYBottom)) {

                mBarPressed = true;
                ret |= OGL_WIDGET_RET_FOCUS_GET;
                if (!utils::inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                                     barXCenter - SCROLL_WIDTH_MULT*barWidth, OGLWidget::mYTop + mBarY,
                                     barXCenter + SCROLL_WIDTH_MULT*barWidth, OGLWidget::mYTop + mBarY + mBarHeight)) {

                    if (updateBarStatus(OGL_WIDGET_MOUSE_GET_Y(event) - OGLWidget::mYTop - 0.5f*mBarHeight)) {
                        mBarHover = true;
                        ret |= OGL_WIDGET_RET_DRAW;
                    }
                }
                mLastYClick = OGL_WIDGET_MOUSE_GET_Y(event) - OGLWidget::mYTop - mBarY;
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
            float maxOffset = ((OGLWidget::mYBottom - OGLWidget::mYTop) - mBarHeight)/mMaxRangeValue;
            if (updateBarStatus(mBarY - (OGL_WIDGET_MOUSE_GET_DELTA(event)*SCROLL_ADVANCE*maxOffset))) {
                ret |= OGL_WIDGET_RET_DRAW;
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
              OGL_WIDGET_SIZE;
        break;

    case OGL_WIDGET_SIZE:
        OGLWidget::onEvent(event);
        //EN UN EVENTO SIZE NO ES NECESARIO NOTIFICAR
        //QUE SE NECECITA ACTUALIZAR EL CONTENIDO
        mBarHover = false;
        if (mBarPressed) {
            mBarPressed = false;
            ret |= OGL_WIDGET_RET_FOCUS_RELEASE;
        }

        float widgetHeight = OGLWidget::mYBottom - OGLWidget::mYTop;
        mBarHeight = (widgetHeight*widgetHeight)/mMaxRangeValue;
        if (mBarHeight >= widgetHeight) {
            mBarHeight = widgetHeight;
            mBarDisable = true;
        } else {
            mBarDisable = false;
            updateBarStatus((mCurrentValue*(widgetHeight - mBarHeight))/(mMaxRangeValue - widgetHeight));
        }
        break;
    }

    return ret;
}
