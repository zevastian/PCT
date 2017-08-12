#include"OGLImage.h"

OGLImage::OGLImage(OGLWidgetDescription description) : OGLWidget(description)
{
    //NADA
}

OGLImage::~OGLImage()
{

}

int OGLImage::onEvent(OGLWidgetEvent event)
{
    OGLWidget::onEvent(event);
    float width = OGLWidget::mXRight - OGLWidget::mXLeft;
    float prop1 = width*mTextureHeight;
    float height = OGLWidget::mYBottom - OGLWidget::mYTop;
    float prop2 = height*mTextureWidth;

    if (event.type == OGL_WIDGET_SIZE) {
        if (prop1 > prop2) {

            if (mScale == OGL_IMAGE_ADJUST) {
                mImageWidth = prop2/mTextureHeight;
                mImageX = OGLWidget::mXLeft + 0.5f*(width - mImageWidth);
            } else if (mScale == OGL_IMAGE_FILL) {
                mImageHeight = prop1/mTextureWidth;
                mUVTextureOffset = 0.5f - 0.5f*(height/mImageHeight);
            }
        } else {

            if (mScale == OGL_IMAGE_ADJUST) {
                mImageHeight = prop1/mTextureWidth;
                mImageY = OGLWidget::mYTop + 0.5f*(height - mImageHeight);
            } else if (mScale == OGL_IMAGE_FILL) {
                mImageWidth = prop2/mTextureHeight;
                mUVTextureOffset = 0.5f - 0.5f*(width/mImageWidth);
            }
        }

    } else if (event.type == OGL_WIDGET_DRAW) {

        if (mScale == OGL_IMAGE_STRETCH) {
            //DRAW(OGLWidget::mXLeft, OGLWidget::mYTop, width, height, 0.0f, 0.0f, 1.0f, 1.0f);
        } else {
            if (prop1 > prop2) {

                if (mScale == OGL_IMAGE_ADJUST) {
                    //DRAW(mImageX, OGLWidget::mYTop, mImageWidth, height, 0.0f, 0.0f, 1.0f, 1.0f);
                } else {
                    //DRAW(OGLWidget::mXLeft, OGLWidget::mYTop, width, height, 0.0f, mUVTextureOffset, 1.0f, 1.0f - mUVTextureOffset);
                }
            } else {
                if (mScale == OGL_IMAGE_ADJUST) {
                    //DRAW(OGLWidget::mXLeft, mImageY, width, mImageHeight, 0.0f, 0.0f, 1.0f, 1.0f);
                } else {
                    //DRAW(OGLWidget::mXLeft, OGLWidget::mYTop, width, height, mUVTextureOffset, 0.0f, 1.0f - mUVTextureOffset, 1.0f);
                }
            }
        }
    }

    return OGL_WIDGET_RETURN_NONE;
}
