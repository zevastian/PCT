#include<cmath>
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
    mFocusedItem = nullptr;
    /******************************************************************************/
    //PARA PROBAR
    for (int i = 0; i < 50; i++) {
        mItems.push_back(std::shared_ptr<OGLTileviewItem>(new OGLTileviewItem));
    }
}

OGLTileview::~OGLTileview()
{

}

void OGLTileview::calculateVisibleItems()
{
    //CALCULAR PRIMER ELEMENTO VISIBLE
    unsigned int rowFirst = std::floor(mScrollbar->getValue()/(mItemHeight + mItemOffset));
    mFirstItem = rowFirst*mNumColumns;
    //CALCULAR ULTIMO ELEMENTO VISIBLE
    float widgetHeight = OGLWidget::getYBottom() - OGLWidget::getYTop();
    unsigned int rowFinish = std::floor((widgetHeight - mItemOffset + mScrollbar->getValue())/(mItemHeight + mItemOffset));
    mFinishItem = mFirstItem + (rowFinish - rowFirst + 1)*mNumColumns;
    if (mFinishItem > mItems.size()) {
        mFinishItem = mItems.size();
    }
}

void OGLTileview::updateItemsPosition()
{
    OGLWidgetEvent event;
    unsigned int rowFirst = std::floor(mScrollbar->getValue()/(mItemHeight + mItemOffset));
    event.data.move.x = OGLWidget::getXLeft() + mItemOffset;
    event.data.move.y = OGLWidget::getYTop() + mItemOffset + rowFirst*(mItemHeight + mItemOffset) - mScrollbar->getValue();
    event.type = OGL_WIDGET_MOVE;

    for (unsigned int i = mFirstItem; i < mFinishItem; i++) {
        mItems[i]->onEvent(event);

        event.data.move.x += mItemWidth + mItemOffset;
        //El 6 CORRESPONDE AL ANCHO DEL SCROLLBAR
        if (event.data.move.x > (OGLWidget::getXRight() - 6.0f - mItemOffset)) {
            event.data.move.x = OGLWidget::getXLeft() + mItemOffset;
            event.data.move.y += mItemHeight + mItemOffset;
        }
    }
}

void OGLTileview::updateItemsSize()
{
    OGLWidgetEvent event;
    event.type = OGL_WIDGET_SIZE;
    event.data.size.width = mItemWidth;
    event.data.size.height = mItemHeight;
    //NOTIFICAR A TODOS LOS ITEMS
    for (unsigned int i = 0; i < mItems.size(); i++) {
        mItems[i]->onEvent(event);
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
        //EXPERIMENTAL
        {
            /******************************************************************************/
            float target = mNumColumns*(mScrollbar->getValue()/(mItemHeight + mItemOffset));
            /******************************************************************************/
            //El 6 CORRESPONDE AL ANCHO DEL SCROLLBAR
            float availableWidth = OGLWidget::getXRight() - OGLWidget::getXLeft() - 6.0f - mItemOffset;
            mNumColumns = std::floor(availableWidth/(mItemMinWidth + mItemOffset));
            mItemWidth = availableWidth/mNumColumns - mItemOffset;
            mItemHeight = 1.5f*mItemWidth;
            updateItemsSize();
            /******************************************************************************/
            mScrollbar->setMaxRangeValue((mItemHeight + mItemOffset)*std::ceil(((float)mItems.size())/mNumColumns));
            /******************************************************************************/
            ev.type = OGL_WIDGET_SIZE;
            ev.data.size.width = OGLWidget::getXRight() - OGLWidget::getXLeft();
            ev.data.size.height = OGLWidget::getYBottom() - OGLWidget::getYTop();
            mScrollbar->onEvent(ev);
            mScrollbar->setValue((target/mNumColumns)*(mItemHeight + mItemOffset));
            mScrollbar->onEvent(ev);
            calculateVisibleItems();
            updateItemsPosition();
            /******************************************************************************/
        }
        //HACER QUE NINGUN ITEM QUEDE SELECCIONADO
        //INDEPENDIENTEMENTE DE SI OGL_SIZE
        //CAMBIA ALGO
        if (mFocusedItem) {
            OGLWidgetEvent event;
            //LEER OBSERVACION DE ABAJO
            event.type = OGL_WIDGET_FOCUS_RELEASE;
            mFocusedItem->onEvent(event);
            mFocusedItem = nullptr;
            ret |= OGL_WIDGET_RET_DRAW;
        }
        break;

    case OGL_WIDGET_MOUSE_MOVE:
        ret = mScrollbar->onEvent(event);
        if (ret & OGL_WIDGET_RET_SCROLL_CHANGE_VALUE) {
            ret &= ~OGL_WIDGET_RET_SCROLL_CHANGE_VALUE;
            calculateVisibleItems();
            updateItemsPosition();
        }

        if (!mBarPressed) {
            bool getFocus = false;
            for (unsigned int i = mFirstItem; i < mFinishItem; i++) {
                //NO USAR FOCUS_GET PORQUE EL WIDGET NO TOMA EL FOCO
                //SOLAMENTE EL MOUSE SE POSICIONA ARRIBA DE EL
                if (mItems[i]->onEvent(event) & OGL_WIDGET_RET_FOCUS_GET) {
                    getFocus = true;
                    if (mFocusedItem != mItems[i]) {
                        if (mFocusedItem) {
                            OGLWidgetEvent event;
                            //NO USAR FOCUS_RELEASE PORQUE ESTE INDICA
                            //QUE EL WIDGET PIERDE EL FOCO Y ACA
                            //SE ESTA USANDO PARA INDICARLE QUE LO PIERDA
                            event.type = OGL_WIDGET_FOCUS_RELEASE;
                            mFocusedItem->onEvent(event);
                        }
                        mFocusedItem = mItems[i];
                        ret |= OGL_WIDGET_RET_DRAW;
                    }
                    break;
                }
            }
            if (!getFocus && mFocusedItem) {
                //SE DEBERIA NOTIFICAR AL WIDGET mFocusedItem
                //SI ESTE ES DISTINTO DE NULL?
                mFocusedItem = nullptr;
                ret |= OGL_WIDGET_RET_DRAW;
            }
        }
        break;

    case OGL_WIDGET_MOUSE_CLICK_DOWN:
        ret = mScrollbar->onEvent(event);
        if (ret & OGL_WIDGET_RET_FOCUS_GET) {
            ret &= ~OGL_WIDGET_RET_FOCUS_GET;
            mBarPressed = true;
            //NOTIFICAR AL WIDGET mFocusedItem?
            if (ret & OGL_WIDGET_RET_SCROLL_CHANGE_VALUE) {
                ret &= ~OGL_WIDGET_RET_SCROLL_CHANGE_VALUE;
                calculateVisibleItems();
                updateItemsPosition();
            }
            break;
        }
        //CREO QUE ESTO SOLO FUNCIONA BIEN SI SIEMPRE
        //ANTES DE UN CLICK_DOWN SE NOTIFICA CON MOUSE_MOVE
        //CUYAS COORDENADAS SON SIMILARES
        if (mFocusedItem) {
            //LLAMAR CALLBACK
        }
        break;

    case OGL_WIDGET_MOUSE_LEAVE:
    case OGL_WIDGET_FOCUS_RELEASE:
        ret = mScrollbar->onEvent(event);
        if (ret & OGL_WIDGET_RET_FOCUS_RELEASE) {
            ret &= ~OGL_WIDGET_RET_FOCUS_RELEASE;
            mBarPressed = false;
        }
        if (mFocusedItem) {
            OGLWidgetEvent event;
            //LEER OBSERVACION DE ARRIBA
            event.type = OGL_WIDGET_FOCUS_RELEASE;
            mFocusedItem->onEvent(event);
            mFocusedItem = nullptr;
            ret |= OGL_WIDGET_RET_DRAW;
        }
        break;

    case OGL_WIDGET_MOVE:
        //NOTIFICAR A LOS WIDGETS?
        ret = OGLWidget::onEvent(event);
        break;

    case OGL_WIDGET_DRAW:
        mScrollbar->onEvent(event);
        //LOS ITEMS SE ENCARGAN DE PINTARSE
        for (unsigned int i = mFirstItem; i < mFinishItem; i++) {
            mItems[i]->onEvent(event);
        }
        break;

    case OGL_WIDGET_REQUEST_EVENT_MASK:
        ret = OGLWidget::onEvent(event) | mScrollbar->onEvent(event) |
              OGL_WIDGET_SIZE | OGL_WIDGET_MOUSE_MOVE | OGL_WIDGET_MOVE |
              OGL_WIDGET_MOUSE_CLICK_DOWN | OGL_WIDGET_MOUSE_LEAVE |
              OGL_WIDGET_DRAW | OGL_WIDGET_FOCUS_RELEASE;
        break;

    default:
        ret = mScrollbar->onEvent(event);
        if (ret & OGL_WIDGET_RET_FOCUS_RELEASE) {
            ret &= ~OGL_WIDGET_RET_FOCUS_RELEASE;
            mBarPressed = false;
        }
        if (ret & OGL_WIDGET_RET_SCROLL_CHANGE_VALUE) {
            ret &= ~OGL_WIDGET_RET_SCROLL_CHANGE_VALUE;
            calculateVisibleItems();
            updateItemsPosition();

            //CUANDO CAMBIA EL VALOR DEL SCROLL
            //PARA QUE NO QUEDE NINGUN ITEM "SELECCIONADO", LO NOTIFICO
            //DEBERIA HACER LO MISMO CON ANIMATE
            if (mFocusedItem) {
                OGLWidgetEvent event;
                //LEER OBSERVACION DE ARRIBA
                event.type = OGL_WIDGET_FOCUS_RELEASE;
                mFocusedItem->onEvent(event);
                mFocusedItem = nullptr;
                ret |= OGL_WIDGET_RET_DRAW;
            }
        }
        break;
    }

    return ret;
}
