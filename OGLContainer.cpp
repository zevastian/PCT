#include"OGLContainer.h"

OGLContainer::OGLContainer(OGLWidgetDescription description) : OGLWidget(description)
{

}

OGLContainer::~OGLContainer()
{

}

int OGLContainer::onEvent(OGLWidgetEvent event)
{
    float lastXLeft;
    float lastYTop;

    switch (event.type) {
    case OGL_WIDGET_SIZE:
        lastXLeft = OGLWidget::getXLeft();
        lastYTop = OGLWidget::getYTop();
        OGLWidget::onEvent(event);
        event.data.size.width = OGLWidget::getXRight() - OGLWidget::getXLeft();
        event.data.size.height = OGLWidget::getYBottom() - OGLWidget::getYTop();
        if (lastXLeft != OGLWidget::getXLeft() || lastYTop != OGLWidget::getYTop()) {

            OGLWidgetEvent ev;
            ev.type = OGL_WIDGET_MOVE;
            ev.data.move.x = OGLWidget::getXLeft();
            ev.data.move.y = OGLWidget::getYTop();
            for (auto it = mChildrens.begin(); it != mChildrens.end(); it++) {
                if (it->eventMask & ev.type) {
                    it->widget->onEvent(ev);
                }
            }
        }
        break;

    case OGL_WIDGET_MOVE:
        OGLWidget::onEvent(event);
        return OGL_WIDGET_RET_NONE;

    case OGL_WIDGET_MOUSE_MOVE:
    case OGL_WIDGET_MOUSE_CLICK_UP:
    case OGL_WIDGET_MOUSE_CLICK_DOWN:
    case OGL_WIDGET_MOUSE_WHEEL:
        //YA NO HACE FALTA AJUSTAR LAS COORDENADAS DEL CURSOR
        //event.data.mouse.x += OGLWidget::getXLeft();
        //event.data.mouse.y += OGLWidget::getYTop();
        break;

    case OGL_WIDGET_REQUEST_EVENT_MASK:
        int mask = OGL_WIDGET_RET_NONE;
        for (auto it = mChildrens.begin(); it != mChildrens.end(); it++) {
            //NO CONTROLAR MASK
            mask |= it->eventMask;
        }
        return mask;
    }

    for (auto it = mChildrens.begin(); it != mChildrens.end(); it++) {
        //HACE FALTA QUE eventMask CONTENGA DRAW, ANIMATE, ETC?
        if (it->eventMask & event.type) {
            it->widget->onEvent(event);
            //FALTA CONTROLAR EL RESULTADO
            //RETORNADO POR EL WIDGET
        }
    }

    return OGL_WIDGET_RET_NONE;
}

void OGLContainer::add(std::shared_ptr<OGLIWidget> widget)
{
    Children child;
    child.widget = widget;

    OGLWidgetEvent event;
    event.type = OGL_WIDGET_REQUEST_EVENT_MASK;
    child.eventMask = widget->onEvent(event);

    mChildrens.push_back(child);
}
