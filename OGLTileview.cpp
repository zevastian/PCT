#include<cmath>
#include"OGLUtils.h"
#include"OGLTileview.h"

OGLTileview::OGLTileview(OGLTileviewDescription description) : OGLWidget(description.widget)
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
    //FALTA CHEQUEAR
    mItemMinWidth = description.tileview.minWidthItem;
    mItemOffset = description.tileview.offsetItem;
    mItemSelected = -1;
}

OGLTileview::~OGLTileview()
{

}

void OGLTileview::forEachVisibleItem(std::function<void (int x, int y, int indx)> fnc)
{
    //CALCULAR PRIMER ELEMENTO VISIBLE
    unsigned int rowFirst = std::floor(mScrollbar->getValue()/(mItemHeight + mItemOffset));
    unsigned int first = rowFirst*mNumColumns;
    //CALCULAR ULTIMO ELEMENTO VISIBLE
    int widgetWidth = OGLWidget::getXRight() - OGLWidget::getXLeft();
    unsigned rowFinish = std::ceil((widgetWidth - mItemOffset + mScrollbar->getValue())/(mItemHeight + mItemOffset));
    unsigned int finish = first + (rowFinish - rowFirst)*mNumColumns;
    if (finish > mNumItems) {
        finish = mNumItems;
    }

    float x = OGLWidget::getXLeft() + mItemOffset;
    float y = OGLWidget::getYTop() + mItemOffset + rowFirst*(mItemHeight + mItemOffset) - mScrollbar->getValue();
    for (unsigned int i = first; i < finish; i++) {
        //
        fnc(x, y, i);

        x += mItemWidth + mItemOffset;
        //El 6 CORRESPONDE AL ANCHO DE EL SCROLLBAR
        if (x > (OGLWidget::getXLeft() + widgetWidth - 6.0f - mItemOffset)) {
            x = OGLWidget::getXLeft() + mItemOffset;
            y += mItemHeight + mItemOffset;
        }
    }
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

    case OGL_WIDGET_MOUSE_MOVE:
        ret = mScrollbar->onEvent(event);
        if (!mBarFocused) {
            bool selected = false;
            forEachVisibleItem([event, &ret, &selected, this](int x, int y, int index) {
                if (utils::inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                                    x, y, x + mItemWidth, y + mItemHeight)) {

                    selected = true;
                    if (index != mItemSelected) {
                        mItemSelected = index;
                        ret |= OGL_WIDGET_RET_DRAW;
                    }
                }
            });
            //MEJORAR REPITO CODIGO
            if (!selected) {
                if (mItemSelected != -1) {
                    mItemSelected = -1;
                    ret |= OGL_WIDGET_RET_DRAW;
                }
            }
        }
        if (mItemSelected != -1) {
            mItemSelected = -1;
            ret |= OGL_WIDGET_RET_DRAW;
        }
        break;

    case OGL_WIDGET_MOUSE_CLICK_DOWN:
        ret = mScrollbar->onEvent(event);
        forEachVisibleItem([event, this](int x, int y, int index) {
            if (utils::inToRect(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event),
                                x, y, x + mItemWidth, y + mItemHeight)) {

                //LLAMAR CALLBACK
            }

        });
        break;

    case OGL_WIDGET_MOUSE_LEAVE:
    case OGL_WIDGET_FOCUS_RELEASE:
        ret = mScrollbar->onEvent(event);
        if (mItemSelected != -1) {
            mItemSelected = -1;
            ret |= OGL_WIDGET_RET_DRAW;
        }
        break;

    case OGL_WIDGET_MOVE:
        ret = OGLWidget::onEvent(event);
        break;

    case OGL_WIDGET_DRAW:
        mScrollbar->onEvent(event);
        break;

    case OGL_WIDGET_REQUEST_EVENT_MASK:
        ret = OGLWidget::onEvent(event) | mScrollbar->onEvent(event) |
              OGL_WIDGET_SIZE | OGL_WIDGET_MOUSE_MOVE | OGL_WIDGET_MOVE |
              OGL_WIDGET_MOUSE_CLICK_DOWN | OGL_WIDGET_MOUSE_LEAVE |
              OGL_WIDGET_DRAW | OGL_WIDGET_FOCUS_RELEASE;
        break;

    default:
        ret = mScrollbar->onEvent(event);
        break;
    }

    return ret;
}
