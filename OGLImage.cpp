#include<epoxy/gl.h>
#include"OGLImage.h"

OGLImage::OGLImage(OGLImageDescription description) : OGLWidget(description.widget)
{
    //FALTA CHEQUEAR CAMPOS
    mDescription = description;
    if (mDescription.image.scale == OGL_IMAGE_STRETCH || mDescription.image.scale == OGL_IMAGE_ADJUST) {
        mTextureUVLeft = 0.0f;
        mTextureUVTop = 0.0f;
        mTextureUVRight = 1.0f;
        mTextureUVBottom = 1.0f;
    }
}

OGLImage::~OGLImage()
{

}

int OGLImage::onEvent(OGLWidgetEvent event)
{
    OGLWidget::onEvent(event);
    float width = OGLWidget::mXRight - OGLWidget::mXLeft;
    float prop1 = width*mDescription.texture.height;
    float height = OGLWidget::mYBottom - OGLWidget::mYTop;
    float prop2 = height*mDescription.texture.width;

    if (event.type == OGL_WIDGET_SIZE) {

        switch (mDescription.image.scale) {
        case OGL_IMAGE_ADJUST:
            if (prop1 > prop2) {
                mImageWidth = prop2/mDescription.texture.height;
                mImageX = OGLWidget::mXLeft + 0.5f*(width - mImageWidth);
                mImageY = OGLWidget::mYTop;
                mImageHeight = height;
            } else {
                mImageX = OGLWidget::mXLeft;
                mImageWidth = width;
                mImageHeight = prop1/mDescription.texture.width;
                mImageY = OGLWidget::mYTop + 0.5f*(height - mImageHeight);
            }
            break;

        case OGL_IMAGE_FILL:
            if (prop1 > prop2) {
                float textureUVOffset = 0.5f - 0.5f*mDescription.texture.width*(height/prop1);
                mTextureUVLeft = 0.0f;
                mTextureUVTop = textureUVOffset;
                mTextureUVRight = 1.0f;
                mTextureUVBottom = 1.0f - textureUVOffset;
            } else {
                float textureUVOffset = 0.5f - 0.5f*mDescription.texture.height*(width/prop2);
                mTextureUVLeft = textureUVOffset;
                mTextureUVTop = 0.0f;
                mTextureUVRight = 1.0f - textureUVOffset;
                mTextureUVBottom = 1.0f;
            }

        case OGL_IMAGE_STRETCH:
            mImageX = OGLWidget::mXLeft;
            mImageWidth = width;
            mImageY = OGLWidget::mYTop;
            mImageHeight = height;
            break;
        }
    } else if (event.type == OGL_WIDGET_DRAW) {
        //ESTO LUEGO DEBERIA TENER UNA IMPLEMENTACION
        //INDEPENDIENTE DE LA PLATAFORMA
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, mDescription.texture.texture);
        glBegin(GL_QUADS);

        /*left-top*/
        glTexCoord2f(mTextureUVLeft, mTextureUVTop);
        glVertex2f(mImageX, mImageY);
        /*right-top*/
        glTexCoord2f(mTextureUVRight, mTextureUVTop);
        glVertex2f(mImageX + mImageWidth, mImageY);
        /*rigth-buttom*/
        glTexCoord2f(mTextureUVRight, mTextureUVBottom);
        glVertex2f(mImageX + mImageWidth, mImageY + mImageHeight);
        /*left-buttom*/
        glTexCoord2f(mTextureUVLeft, mTextureUVBottom);
        glVertex2f(mImageX, mImageY + mImageHeight);

        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    return OGL_WIDGET_RET_NONE;
}
