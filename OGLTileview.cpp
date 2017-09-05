#include"OGLTileview.h"
#include<iostream>

OGLTileview::OGLTileview(OGLWidgetDescription description) : OGLWidget(description)
{
    /******************************************************************************/
    OGLWidgetDescription desc;
    desc.x.flag = OGLWidgetXFlag::OGL_ALIGN_RIGHT;
    desc.y.flag = OGLWidgetYFlag::OGL_ALIGN_TOP;
    desc.width.value = 6.0f;
    desc.width.flag = OGLWidgetDimensionFlag::OGL_PX;
    desc.height.value = 100.0f;
    desc.height.flag = OGLWidgetDimensionFlag::OGL_PERCENT;
    mScrollbar = std::shared_ptr<OGLScrollbar> (new OGLScrollbar(desc));
    //SE SUPONE QUE SCROLLBAR TIENE QUE INICIALIZARLO
    mScrollbar->setMaxRangeValue(0.0f);
    mScrollbar->setValue(0.0f);
    /******************************************************************************/
}

OGLTileview::~OGLTileview()
{

}

int OGLTileview::onEvent(OGLWidgetEvent event)
{
    int ret = OGL_WIDGET_RET_NONE;
    switch (event.type) {

    case OGL_WIDGET_SIZE:
        OGLWidget::onEvent(event);
        OGLWidgetEvent ev;
        //SCROLLBAR NO ESTA RECIBIENDO UN OGL_WIDGET_MOVE
        //PORQUE NO HAY UN CAMBIO DE POSICION DE TILEVIEW
        ev.type = OGL_WIDGET_MOVE;
        ev.data.move.x = OGLWidget::getXLeft();
        ev.data.move.y = OGLWidget::getYTop();
        mScrollbar->onEvent(ev);
        //mScrollbar->setMaxRangeValue();
        //mScrollbar->setValue();
        ev.type = OGL_WIDGET_SIZE;
        ev.data.size.width = OGLWidget::getXRight() - OGLWidget::getXLeft();
        ev.data.size.height = OGLWidget::getYBottom() - OGLWidget::getYTop();
        mScrollbar->onEvent(ev);
        //mScrollbar->getValue();
        break;

    case OGL_WIDGET_MOVE:
        OGLWidget::onEvent(event);
        break;

    case OGL_WIDGET_DRAW:
        mScrollbar->onEvent(event);
        break;

    case OGL_WIDGET_REQUEST_EVENT_MASK:
        ret = OGLWidget::onEvent(event) | mScrollbar->onEvent(event) |
              OGL_WIDGET_SIZE | OGL_WIDGET_MOVE |
              OGL_WIDGET_DRAW;
        break;

    default:
        ret = mScrollbar->onEvent(event);
        break;
    }

    return ret;
}
