#include<epoxy/gl.h>
#include<cmath>
#include"OGLRoundedButton.h"
#include"OGLUtils.h"

OGLRoundedButton::OGLRoundedButton(OGLRoundedButtonDescription description) : OGLWidget(description.widget)
{
    //FALTA CHEQUEAR CAMPOS
    onClick = description.button.onClick;
    mRadius = description.button.radius;
}

OGLRoundedButton::~OGLRoundedButton()
{

}

bool OGLRoundedButton::overButton(int x, int y)
{
    //PRIMERO HAGO UNA COMPROBACION BASICA
    if (utils::inToRect(x, y,
                        OGLWidget::mXLeft, OGLWidget::mYTop,
                        OGLWidget::mXRight, OGLWidget::mYBottom)) {
        //RECTANGULO DEL CENTRO
        if (utils::inToRect(x, y,OGLWidget::mXLeft, OGLWidget::mYTop + mRadius,
                            OGLWidget::mXRight, OGLWidget::mYBottom - mRadius) ||
                //RECTANGULO SUPERRIOR
                utils::inToRect(x, y, OGLWidget::mXLeft + mRadius, OGLWidget::mYTop,
                                OGLWidget::mXRight - mRadius, OGLWidget::mYTop + mRadius) ||
                //RECTANGULO INFERIOR
                utils::inToRect(x, y, OGLWidget::mXLeft + mRadius, OGLWidget::mYBottom - mRadius,
                                OGLWidget::mXRight - mRadius, OGLWidget::mYBottom) ||
                //CIRCULO SUPERIOR IZQUIERDO
                utils::inToCircle(x, y, OGLWidget::mXLeft + mRadius,
                                  OGLWidget::mYTop + mRadius, mRadius) ||
                //CIRCULO INFERIOR IZQUIERDO
                utils::inToCircle(x, y, OGLWidget::mXLeft + mRadius,
                                  OGLWidget::mYBottom - mRadius, mRadius) ||
                //CIRCULO INFERIOR DERECHO
                utils::inToCircle(x, y, OGLWidget::mXRight - mRadius,
                                  OGLWidget::mYBottom - mRadius, mRadius) ||
                //CIRCULO SUPERIOR DERECHO
                utils::inToCircle(x, y, OGLWidget::mXRight - mRadius,
                                  OGLWidget::mYTop + mRadius, mRadius)) {

            return true;
        }
    }
    return false;
}

void OGLRoundedButton::drawArc(float x, float y, float offset, float arc, int steps)
{
    //ESTO EN UN FUTURO SERA ALGUN METODO DE GrDevice
    glBegin(GL_TRIANGLE_FAN);
    /*center vertex*/
    glVertex2f(x, y);
    /*arc vertexs*/
    for (int i = 0; i <= steps; i++) {
        float angle = offset + arc*(i/(float)steps);
        float xArc = x + mRadius*cos(angle);
        float yArc = y - mRadius*sin(angle);
        glVertex2f(xArc, yArc);
    }
    glEnd();
}

void OGLRoundedButton::draw()
{
    //ESTO TENDRA UNA IMPLEMENTACION INDEPENDIENTE
    //EN UN FUTURO
    if (!mHover) {
        glColor4f(0.914f, 0.329f, 0.125f, 1.0f);
    } else {
        glColor4f(0.929f, 0.463f, 0.302f, 1.0f);
    }

    glBegin(GL_QUADS);
    //RECTANGULO DEL CENTRO
    glVertex2f(OGLWidget::mXLeft, OGLWidget::mYTop + mRadius);
    glVertex2f(OGLWidget::mXRight, OGLWidget::mYTop + mRadius);
    glVertex2f(OGLWidget::mXRight, OGLWidget::mYBottom - mRadius);
    glVertex2f(OGLWidget::mXLeft, OGLWidget::mYBottom - mRadius);
    //RECTANGULO SUPERIOR
    glVertex2f(OGLWidget::mXLeft + mRadius, OGLWidget::mYTop);
    glVertex2f(OGLWidget::mXRight - mRadius, OGLWidget::mYTop);
    glVertex2f(OGLWidget::mXRight - mRadius, OGLWidget::mYTop + mRadius);
    glVertex2f(OGLWidget::mXLeft + mRadius, OGLWidget::mYTop + mRadius);
    //RECTANGULO INFERIOR
    glVertex2f(OGLWidget::mXLeft + mRadius, OGLWidget::mYBottom - mRadius);
    glVertex2f(OGLWidget::mXRight - mRadius, OGLWidget::mYBottom - mRadius);
    glVertex2f(OGLWidget::mXRight - mRadius, OGLWidget::mYBottom);
    glVertex2f(OGLWidget::mXLeft + mRadius, OGLWidget::mYBottom);
    glEnd();

    //CALCULO CANTIDAD DE VERTICES
    //std::ceil(2.0f*sqrt(radius)*(arc/PI)
    int steps = std::ceil(sqrt(mRadius));

    #define PI 3.141592654f
    #define PI_DIV_2 1.570796327f
    #define PI_DIV_4 0.785398163f

    //CORNER SUPERIOR DERECHO
    drawArc(OGLWidget::mXRight - mRadius, OGLWidget::mYTop + mRadius, 0.0f, PI_DIV_2, steps);
    //CORNER SUPERIOR IZQUIERDO
    drawArc(OGLWidget::mXLeft + mRadius, OGLWidget::mYTop + mRadius, PI_DIV_2, PI_DIV_2, steps);
    //CORNER INFERIOR IZQUIERDO
    drawArc(OGLWidget::mXLeft + mRadius, OGLWidget::mYBottom - mRadius, PI, PI_DIV_2, steps);
    //CORNER INFERIOR DERECHO
    drawArc(OGLWidget::mXRight - mRadius, OGLWidget::mYBottom - mRadius, PI + PI_DIV_2, PI_DIV_2, steps);
}

int OGLRoundedButton::onEvent(OGLWidgetEvent event)
{
    int ret = OGL_WIDGET_RET_NONE;

    switch (event.type) {
    case OGL_WIDGET_MOUSE_MOVE:
        if (overButton(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event))) {
            if (!mHover) {
                mHover = true;
                ret |= OGL_WIDGET_RET_DRAW;
            }
        } else {
            if (mHover) {
                mHover = false;
                ret |= OGL_WIDGET_RET_DRAW;
            }
        }
        break;

    case OGL_WIDGET_MOUSE_LEAVE:
    case OGL_WIDGET_FOCUS_RELEASE:
        if (mHover) {
            mHover = false;
            ret |= OGL_WIDGET_RET_DRAW;
        }
        break;

    case OGL_WIDGET_MOUSE_CLICK_DOWN:
        if (overButton(OGL_WIDGET_MOUSE_GET_X(event), OGL_WIDGET_MOUSE_GET_Y(event))) {
            onClick();
            mHover = true;
            ret |= OGL_WIDGET_RET_DRAW;
        }
        break;

    case OGL_WIDGET_DRAW:
        draw();
        break;

    case OGL_WIDGET_SIZE:
        OGLWidget::onEvent(event);
        //NO ES NECESARIO SOLICITAR UN DRAW
        break;
    }

    return ret;
}
