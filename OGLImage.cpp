#include"OGLImage.h"

OGLImage::OGLImage(OGLWidgetDescription description) : OGLWidget(description)
{
    //NADA
}

OGLImage::~OGLImage()
{

}

void OGLImage::onEvent(OGLWidgetEvent& event)
{
    OGLWidget::onEvent(event);
    if (event.type == OGL_WIDGET_SIZE) {

        float width = OGLWidget::mXRight - OGLWidget::mXLeft;
        float var1 = width*mHeightTexture;
        float height = OGLWidget::mYBottom - OGLWidget::mYTop;
        float var2 = height*mWidthTexture;

        if (var1 > var2) {

            switch (mScale) {
            case OGL_IMAGE_ADJUST:
                mImageWidth = var2/mHeightTexture;
                mImageX = OGLWidget::mXLeft + 0.5f*(width - mImageWidth);
                break;

            case OGL_IMAGE_FILL:
                mImageHeight = var1/mWidthTexture;
                mUVTextureOffset = 0.5f - 0.5f*(height/mImageHeight);
                break;
            }

        } else {

            switch (mScale) {
            case OGL_IMAGE_ADJUST:
                mImageHeight = var1/mWidthTexture;
                mImageY = OGLWidget::mYTop + 0.5f*(height - mImageHeight);
                break;

            case OGL_IMAGE_FILL:
                mImageWidth = var2/mHeightTexture;
                mUVTextureOffset = 0.5f - 0.5f*(width/mImageWidth);
                break;
            }

        }
    } else if (event.type == OGL_WIDGET_DRAW) {

        switch (mScale) {
        case OGL_IMAGE_STRETCH:
            break;

        case OGL_IMAGE_ADJUST:
            break;

        case OGL_IMAGE_FILL:
            break;
        }
    }
}
