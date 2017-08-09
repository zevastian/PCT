#include"OGLWidget.h"

OGLWidget::OGLWidget()
{

}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::onEvent(OGLWidgetEvent& event)
{
    switch (event.type) {

    case OGL_WIDGET_MOVE:
        break;

    case OGL_WIDGET_SIZE:
        break;
    }
}
