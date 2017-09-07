#include<memory>
#include<functional>
#include"OGLWidget.h"
#include"OGLScrollbar.h"
#include"OGLLabel.h"

#ifndef _OGL_TILEVIEW_H_
#define _OGL_TILEVIEW_H_

struct OGLTileviewDescription {
    OGLWidgetDescription widget;
    struct {
        float minWidthItem;
        float offsetItem;
    } tileview;
    OGLTileviewDescription()
    {
        tileview.minWidthItem = 0.0f;
        tileview.offsetItem = 0.0f;
    }
};

class OGLTileview : public OGLWidget
{
private:
    std::shared_ptr<OGLScrollbar> mScrollbar;
    int mNumColumns;
    float mItemMinWidth;
    float mItemOffset;
    float mItemWidth;
    float mItemHeight;
    int mItemSelected;
    unsigned int mNumItems;
    bool mBarFocused;
    void forEachVisibleItem(std::function<void (float x, float y, int indx)> fnc);

public:
    /**/
    OGLTileview(OGLTileviewDescription description);
    /**/
    ~OGLTileview();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
