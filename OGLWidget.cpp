#include"OGLWidget.h"

OGLWidget::OGLWidget(OGLWidgetDescription description)
{
    //FALTA CHEQUEAR CAMPOS INCOMPLETOS
    mDescription = description;
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
        /******************************************************************************/
        switch (mDescription.x.flag) {
        case OGL_ALIGN_LEFT:
        case OGL_RELATIVE_LEFT:
            mXLeft = (mDescription.x.flag == OGL_RELATIVE_LEFT) ? mDescription.x.value : 0.0f;
            mXRight = mXLeft + ((mDescription.width.flag == OGL_PX) ? mDescription.width.value :
                                //mDescription.width.value REPRESENTA UN PORCENTAJE
                                0.01f*((OGL_WIDGET_SIZE_GET_WIDTH(event) - mXLeft)*mDescription.width.value));
            break;
        /******************************************************************************/
        case OGL_ALIGN_RIGHT:
        case OGL_RELATIVE_RIGHT:
            //mDescription.x.value REPRESENTA LA DISTANCIA RESPECTO DEL MARGEN DERECHO EN UNIDADES DE PIXEL
            mXRight = (mDescription.x.flag == OGL_RELATIVE_RIGHT) ? OGL_WIDGET_SIZE_GET_WIDTH(event) - mDescription.x.value :
                      OGL_WIDGET_SIZE_GET_WIDTH(event);
            mXLeft = mXRight - ((mDescription.width.flag == OGL_PX) ? mDescription.width.value :
                                //mDescription.width.value REPRESENTA UN PORCENTAJE
                                0.01f*(mXRight*mDescription.width.value));
            break;
        /******************************************************************************/
        case OGL_ALIGN_CENTER_X:
            float width = (mDescription.width.flag == OGL_PX) ? mDescription.width.value :
                          //mDescription.width.value REPRESENTA UN PORCENTAJE
                          0.01f*(OGL_WIDGET_SIZE_GET_WIDTH(event)*mDescription.width.value);
            mXLeft = 0.5f*(OGL_WIDGET_SIZE_GET_WIDTH(event) - width);
            mXRight = 0.5f*(OGL_WIDGET_SIZE_GET_WIDTH(event) + width);
            break;
        }
        /******************************************************************************/
        switch (mDescription.y.flag) {
        case OGL_ALIGN_TOP:
        case OGL_RELATIVE_TOP:
            mYTop = (mDescription.y.flag == OGL_RELATIVE_TOP) ? mDescription.y.value : 0.0f;
            mYBottom = mYTop + ((mDescription.height.flag == OGL_PX) ? mDescription.height.value :
                                //mDescription.height.value REPRESENTA UN PORCENTAJE
                                0.01f*((OGL_WIDGET_SIZE_GET_HEIGHT(event) - mYTop)*mDescription.height.value));
            break;
        /******************************************************************************/
        case OGL_ALIGN_BOTTOM:
        case OGL_RELATIVE_BOTTOM:
            //mDescription.y.value REPRESENTA LA DISTANCIA RESPECTO DEL MARGEN INFERIOR EN UNIDADES DE PIXEL
            mYBottom = (mDescription.y.flag == OGL_RELATIVE_BOTTOM) ? OGL_WIDGET_SIZE_GET_HEIGHT(event) - mDescription.y.value :
                       OGL_WIDGET_SIZE_GET_HEIGHT(event);
            mYTop = mYBottom - ((mDescription.height.flag == OGL_PX) ? mDescription.height.value :
                                //mDescription.height.value REPRESENTA UN PORCENTAJE
                                0.01f*(mYBottom*mDescription.height.value));
            break;
        /******************************************************************************/
        case OGL_ALIGN_CENTER_Y:
            float height = (mDescription.height.flag == OGL_PX) ? mDescription.height.value :
                           //mDescription.height.value REPRESENTA UN PORCENTAJE
                           0.01f*(OGL_WIDGET_SIZE_GET_HEIGHT(event)*mDescription.height.value);
            mYTop = 0.5f*(OGL_WIDGET_SIZE_GET_HEIGHT(event) - height);
            mYBottom = 0.5f*(OGL_WIDGET_SIZE_GET_HEIGHT(event) + height);
            break;
        }
        /******************************************************************************/
    }
}
