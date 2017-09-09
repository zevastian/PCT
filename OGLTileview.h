#include<memory>
#include<vector>
#include"OGLWidget.h"
#include"OGLScrollbar.h"
#include"OGLTileviewItem.h"

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
    std::shared_ptr<OGLTileviewItem> mFocusedItem;
    std::vector<std::shared_ptr<OGLTileviewItem>> mItems;
    unsigned int mFirstItem;
    unsigned int mFinishItem;
    int mNumColumns;
    float mItemMinWidth;
    float mItemOffset;
    float mItemWidth;
    float mItemHeight;
    bool mBarFocused;
    void calculateVisibleItems();
    void updateItemsPosition();

public:
    /**/
    OGLTileview(OGLTileviewDescription description);
    /**/
    ~OGLTileview();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
