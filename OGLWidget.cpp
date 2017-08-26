#include"OGLWidget.h"
#include<cfloat>

OGLWidget::OGLWidget(OGLWidgetDescription description)
{
    //FALTA CHEQUEAR CAMPOS INCOMPLETOS
    mDescription = description;
    /******************************************************************************/
    //NO ME QUEDA CALRO PORQUE ESTO SOLUCIONA EL PROBLEMA
    //EN EL QUE ALGUNO WIDGET NO SE MUESTRAN
    mXLeft = FLT_MIN;
    mXRight = FLT_MAX;
    mYTop = FLT_MIN;
    mYBottom = FLT_MAX;
    mXParent = FLT_MIN;
    mYParent = FLT_MIN;
    /******************************************************************************/

    if (mDescription.x.flag == OGL_RELATIVE_LEFT) {
        mXLeft = mDescription.x.value;
    } else if (mDescription.x.flag == OGL_ALIGN_LEFT) {
        mDescription.x.flag = OGL_RELATIVE_LEFT;
        mXLeft = 0.0f;
    } else if (mDescription.x.flag == OGL_ALIGN_RIGHT) {
        mDescription.x.flag = OGL_RELATIVE_RIGHT;
        mDescription.x.value = 0;
    }

    if (mDescription.x.flag == OGL_RELATIVE_LEFT && mDescription.width.flag == OGL_PX) {
        mXRight = mXLeft + mDescription.width.value;
    }

    if (mDescription.y.flag == OGL_RELATIVE_TOP) {
        mYTop = mDescription.y.value;
    } else if (mDescription.y.flag == OGL_ALIGN_TOP) {
        mDescription.y.flag = OGL_RELATIVE_TOP;
        mYTop = 0.0f;
    } else if(mDescription.y.flag == OGL_ALIGN_BOTTOM) {
        mDescription.y.flag = OGL_RELATIVE_BOTTOM;
        mDescription.y.value = 0.0f;
    }

    if (mDescription.y.flag == OGL_RELATIVE_TOP && mDescription.height.flag == OGL_PX) {
        mYBottom = mYTop + mDescription.height.value;
    }
}

OGLWidget::~OGLWidget()
{

}

int OGLWidget::onEvent(OGLWidgetEvent event)
{
    int ret = OGL_WIDGET_RET_NONE;
    switch (event.type) {

    case OGL_WIDGET_MOVE:
        //TODO OGL_WIDGET_MOVE PRECEDE A UN OGL_WIDGET_SIZE
        //CON TOTAL INDEPENDENCIA DE SI CAMBIO O NO
        //EL TAMAÑO DEL WIDGET PARENT
        mXParent = OGL_WIDGET_MOVE_GET_X(event);
        mYParent = OGL_WIDGET_MOVE_GET_Y(event);
        break;

    case OGL_WIDGET_SIZE:
        if (mDescription.width.flag == OGL_PX) {

            if (mDescription.x.flag == OGL_RELATIVE_RIGHT) {
                mXRight = OGL_WIDGET_SIZE_GET_WIDTH(event) - mDescription.x.value;
                mXLeft = mXRight - mDescription.width.value;
            } else if (mDescription.x.flag == OGL_ALIGN_CENTER_X) {
                mXRight = 0.5f*(OGL_WIDGET_SIZE_GET_WIDTH(event) + mDescription.width.value);
                mXLeft = 0.5f*(OGL_WIDGET_SIZE_GET_WIDTH(event) - mDescription.width.value);
            }
        } else {

            if (mDescription.x.flag == OGL_RELATIVE_LEFT) {
                mXRight = mXLeft + 0.01f*(OGL_WIDGET_SIZE_GET_WIDTH(event) - mXLeft)*mDescription.width.value;
            } else if (mDescription.x.flag == OGL_RELATIVE_RIGHT) {
                mXRight = OGL_WIDGET_SIZE_GET_WIDTH(event) - mDescription.x.value;
                mXLeft = mXRight - 0.01f*mXRight*mDescription.width.value;
            } else {
                mXRight = OGL_WIDGET_SIZE_GET_WIDTH(event)*(0.5f + 0.005f*mDescription.width.value);
                mXLeft = OGL_WIDGET_SIZE_GET_WIDTH(event)*(0.5f - 0.005f*mDescription.width.value);
            }
        }

        if (mDescription.height.flag == OGL_PX) {

            if (mDescription.y.flag == OGL_RELATIVE_BOTTOM) {
                mYBottom = OGL_WIDGET_SIZE_GET_HEIGHT(event) - mDescription.y.value;
                mYTop = mYBottom - mDescription.height.value;
            } else if (mDescription.y.flag == OGL_ALIGN_CENTER_Y) {
                mYBottom = 0.5f*(OGL_WIDGET_SIZE_GET_HEIGHT(event) + mDescription.height.value);
                mYTop = 0.5f*(OGL_WIDGET_SIZE_GET_HEIGHT(event) - mDescription.height.value);
            }
        } else {

            if (mDescription.y.flag == OGL_RELATIVE_TOP) {
                mYBottom = mYTop + 0.01f*(OGL_WIDGET_SIZE_GET_HEIGHT(event) - mYTop)*mDescription.height.value;
            } else if (mDescription.y.flag == OGL_RELATIVE_BOTTOM) {
                mYBottom = OGL_WIDGET_SIZE_GET_HEIGHT(event) - mDescription.y.value;
                mYTop = mYBottom - 0.01f*mYBottom*mDescription.height.value;
            } else {
                mYBottom = OGL_WIDGET_SIZE_GET_HEIGHT(event)*(0.5f + 0.005f*mDescription.height.value);
                mYTop = OGL_WIDGET_SIZE_GET_HEIGHT(event)*(0.5f - 0.005f*mDescription.height.value);
            }
        }
        break;

    case OGL_WIDGET_REQUEST_EVENT_MASK:
        ret = OGL_WIDGET_MOVE | OGL_WIDGET_SIZE;
        break;
    }

    return ret;
}

float OGLWidget::getXLeft()
{
    return mXLeft + mXParent;
}

float OGLWidget::getXRight()
{
    return mXRight + mXParent;
}

float OGLWidget::getYTop()
{
    return mYTop + mYParent;
}

float OGLWidget::getYBottom()
{
    return mYBottom + mYParent;
}
