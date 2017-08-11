#include"OGLWidget.h"

OGLWidget::OGLWidget(OGLWidgetDescription description)
{
    //FALTA CHEQUEAR CAMPOS INCOMPLETOS
    mDescription = description;

    if (mDescription.x.flag == OGL_RELATIVE_LEFT) {
        mXLeft = mDescription.x.value;
    } else if (mDescription.x.flag == OGL_ALIGN_LEFT) {
        mXLeft = 0.0f;
    } else if (mDescription.x.flag == OGL_ALIGN_RIGHT) {
        mDescription.x.value = 0;
    }
    if ((mDescription.x.flag == OGL_RELATIVE_LEFT || mDescription.x.flag == OGL_ALIGN_LEFT) && mDescription.width.flag == OGL_PX) {
        mXRight = mXLeft + mDescription.width.value;
    }

    if (mDescription.y.flag == OGL_RELATIVE_TOP) {
        mYTop = mDescription.y.value;
    } else if (mDescription.y.flag == OGL_ALIGN_TOP) {
        mYTop = 0.0f;
    } else if(mDescription.y.flag == OGL_ALIGN_BOTTOM) {
        mDescription.y.value = 0.0f;
    }
    if ((mDescription.y.flag == OGL_RELATIVE_TOP || mDescription.y.flag == OGL_ALIGN_TOP) && mDescription.height.flag == OGL_PX) {
        mYBottom = mYTop + mDescription.height.value;
    }
}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::onEvent(OGLWidgetEvent& event)
{
    switch (event.type) {
    case OGL_WIDGET_MOVE:
        //SI EL PADRE CAMBIA DE POSICION PERO NO DE TAMAÑO
        //COMO REACCIONAN LOS CHILDREN?
        mXParent = OGL_WIDGET_MOVE_GET_X(event);
        mYParent = OGL_WIDGET_MOVE_GET_Y(event);
        break;

    case OGL_WIDGET_SIZE:
        if (mDescription.width.flag == OGL_PX) {
            /******************************************************************************/
            switch (mDescription.x.flag) {
            case OGL_ALIGN_RIGHT:
            case OGL_RELATIVE_RIGHT:
                mXRight = OGL_WIDGET_SIZE_GET_WIDTH(event) - mDescription.x.value;
                mXLeft = mXRight - mDescription.width.value;
                break;

            case OGL_ALIGN_CENTER_X:
                mXLeft = 0.5f*(OGL_WIDGET_SIZE_GET_WIDTH(event) - mDescription.width.value);
                mXRight = 0.5f*(OGL_WIDGET_SIZE_GET_WIDTH(event) + mDescription.width.value);
                break;
            }
            /******************************************************************************/
            switch (mDescription.y.flag) {
            case OGL_ALIGN_BOTTOM:
            case OGL_RELATIVE_BOTTOM:
                mYBottom = OGL_WIDGET_SIZE_GET_HEIGHT(event) - mDescription.y.value;
                mYTop = mYBottom - mDescription.height.value;
                break;

            case OGL_ALIGN_CENTER_Y:
                mYTop = 0.5f*(OGL_WIDGET_SIZE_GET_HEIGHT(event) - mDescription.height.value);
                mYBottom = 0.5f*(OGL_WIDGET_SIZE_GET_HEIGHT(event) + mDescription.height.value);
                break;
            }
            /******************************************************************************/
        } else {
            /******************************************************************************/
            switch (mDescription.x.flag) {
            case OGL_ALIGN_LEFT:
            case OGL_RELATIVE_LEFT:
                mXRight = mXLeft + 0.01f*(OGL_WIDGET_SIZE_GET_WIDTH(event) - mXLeft)*mDescription.width.value;
                break;

            case OGL_ALIGN_RIGHT:
            case OGL_RELATIVE_RIGHT:
                mXRight = OGL_WIDGET_SIZE_GET_WIDTH(event) - mDescription.x.value;
                mXLeft = mXRight - 0.01f*mXRight*mDescription.width.value;
                break;

            case OGL_ALIGN_CENTER_X:
                float width = 0.01f*OGL_WIDGET_SIZE_GET_WIDTH(event)*mDescription.width.value;
                mXLeft = 0.5f*(OGL_WIDGET_SIZE_GET_WIDTH(event) - width);
                mXRight = 0.5f*(OGL_WIDGET_SIZE_GET_WIDTH(event) + width);
                break;
            }
            /******************************************************************************/
            switch (mDescription.y.flag) {
            case OGL_ALIGN_TOP:
            case OGL_RELATIVE_TOP:
                mYBottom = mYTop + 0.01f*(OGL_WIDGET_SIZE_GET_HEIGHT(event) - mYTop)*mDescription.height.value;
                break;

            case OGL_ALIGN_BOTTOM:
            case OGL_RELATIVE_BOTTOM:
                mYBottom = OGL_WIDGET_SIZE_GET_HEIGHT(event) - mDescription.y.value;
                mYTop = mYBottom - 0.01f*mYBottom*mDescription.height.value;
                break;

            case OGL_ALIGN_CENTER_Y:
                float height = 0.01f*OGL_WIDGET_SIZE_GET_HEIGHT(event)*mDescription.height.value;
                mYTop = 0.5f*(OGL_WIDGET_SIZE_GET_HEIGHT(event) - height);
                mYBottom = 0.5f*(OGL_WIDGET_SIZE_GET_HEIGHT(event) + height);
                break;
            }
            /******************************************************************************/
        }
    }
}
