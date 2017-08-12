#include"OGLWidget.h"

#ifndef _OGL_IMAGE_H_
#define _OGL_IMAGE_H_

enum OGL_IMAGE_SCALE {
    OGL_IMAGE_STRETCH,
    OGL_IMAGE_FILL,
    OGL_IMAGE_ADJUST
};

class OGLImage : public OGLWidget
{
private:
    float mImageX;
    float mImageY;
    float mImageWidth;
    float mImageHeight;
    float mTextureWidth;
    float mTextureHeight;
    float mUVTextureOffset;
    OGL_IMAGE_SCALE mScale;

public:
    /**/
    OGLImage(OGLWidgetDescription description);
    /**/
    ~OGLImage();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
