#include"OGLWidget.h"

#ifndef _OGL_IMAGE_H_
#define _OGL_IMAGE_H_

enum OGL_IMAGE_SCALE {
    OGL_IMAGE_SCALE_NONE,
    OGL_IMAGE_STRETCH,
    OGL_IMAGE_FILL,
    OGL_IMAGE_ADJUST
};

struct OGLImageDescription {
    struct {
        GLuint texture;
        unsigned int height;
        unsigned int width;
    } texture;
    struct {
        OGL_IMAGE_SCALE scale;
    } image;
    OGLWidgetDescription widget;
    OGLImageDescription ()
    {
        image.scale = OGL_IMAGE_SCALE_NONE;
        texture.texture = 0;
    }
};

class OGLImage : public OGLWidget
{
private:
    float mImageX;
    float mImageY;
    float mImageWidth;
    float mImageHeight;
    float mTextureUVLeft;
    float mTextureUVTop;
    float mTextureUVRight;
    float mTextureUVBottom;
    OGLImageDescription mDescription;

public:
    /**/
    OGLImage(OGLImageDescription description);
    /**/
    ~OGLImage();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
