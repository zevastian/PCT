#include<epoxy/gl.h>
#include"OGLSquare.h"

OGLSquare::OGLSquare(OGLSquareDescription description) : OGLWidget(description.widget)
{
    //FALTA CHEQUEAR
    mDescription = description;
}

OGLSquare::~OGLSquare()
{

}

int OGLSquare::onEvent(OGLWidgetEvent event)
{
    int ret = OGL_WIDGET_RET_NONE;
    switch (event.type) {

    case OGL_WIDGET_MOVE:
    case OGL_WIDGET_SIZE:
        OGLWidget::onEvent(event);
        break;

    case OGL_WIDGET_DRAW:
        //EN UN FUTURO TENDRA UNA
        //MEJOR IMPLEMENTACION
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glColor4f(mDescription.color.r, mDescription.color.g, mDescription.color.b,
                  mDescription.color.a);

        glBegin(GL_QUADS);
        glVertex2f(OGLWidget::getXLeft(), OGLWidget::getYTop());
        glVertex2f(OGLWidget::getXRight(), OGLWidget::getYTop());
        glVertex2f(OGLWidget::getXRight(), OGLWidget::getYBottom());
        glVertex2f(OGLWidget::getXLeft(), OGLWidget::getYBottom());
        glEnd();

        glDisable(GL_BLEND);
        break;

    case OGL_WIDGET_REQUEST_EVENT_MASK:
        ret = OGLWidget::onEvent(event) | OGL_WIDGET_SIZE |
              OGL_WIDGET_DRAW;
        break;
    }

    return ret;
}
