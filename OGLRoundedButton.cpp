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
                        OGLWidget::getXLeft(), OGLWidget::getYTop(),
                        OGLWidget::getXRight(), OGLWidget::getYBottom())) {
        //RECTANGULO DEL CENTRO
        if (utils::inToRect(x, y,OGLWidget::getXLeft(), OGLWidget::getYTop() + mRadius,
                            OGLWidget::getXRight(), OGLWidget::getYBottom() - mRadius) ||
                //RECTANGULO SUPERRIOR
                utils::inToRect(x, y, OGLWidget::getXLeft() + mRadius, OGLWidget::getYTop(),
                                OGLWidget::getXRight() - mRadius, OGLWidget::getYTop() + mRadius) ||
                //RECTANGULO INFERIOR
                utils::inToRect(x, y, OGLWidget::getXLeft() + mRadius, OGLWidget::getYBottom() - mRadius,
                                OGLWidget::getXRight() - mRadius, OGLWidget::getYBottom()) ||
                //CIRCULO SUPERIOR IZQUIERDO
                utils::inToCircle(x, y, OGLWidget::getXLeft() + mRadius,
                                  OGLWidget::getYTop() + mRadius, mRadius) ||
                //CIRCULO INFERIOR IZQUIERDO
                utils::inToCircle(x, y, OGLWidget::getXLeft() + mRadius,
                                  OGLWidget::getYBottom() - mRadius, mRadius) ||
                //CIRCULO INFERIOR DERECHO
                utils::inToCircle(x, y, OGLWidget::getXRight() - mRadius,
                                  OGLWidget::getYBottom() - mRadius, mRadius) ||
                //CIRCULO SUPERIOR DERECHO
                utils::inToCircle(x, y, OGLWidget::getXRight() - mRadius,
                                  OGLWidget::getYTop() + mRadius, mRadius)) {

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
    //TAL VEZ EN VEZ DE CAMBIAR EL COLOR DEL BOTON
    //CONVIENE CAMBIAR EL MOUSE
    //ESTO TENDRA UNA IMPLEMENTACION INDEPENDIENTE
    //EN UN FUTURO
    if (!mHover) {
        glColor4f(0.914f, 0.329f, 0.125f, 1.0f);
    } else {
        glColor4f(0.929f, 0.463f, 0.302f, 1.0f);
    }

    glBegin(GL_QUADS);
    //RECTANGULO DEL CENTRO
    glVertex2f(OGLWidget::getXLeft(), OGLWidget::getYTop() + mRadius);
    glVertex2f(OGLWidget::getXRight(), OGLWidget::getYTop() + mRadius);
    glVertex2f(OGLWidget::getXRight(), OGLWidget::getYBottom() - mRadius);
    glVertex2f(OGLWidget::getXLeft(), OGLWidget::getYBottom() - mRadius);
    //RECTANGULO SUPERIOR
    glVertex2f(OGLWidget::getXLeft() + mRadius, OGLWidget::getYTop());
    glVertex2f(OGLWidget::getXRight() - mRadius, OGLWidget::getYTop());
    glVertex2f(OGLWidget::getXRight() - mRadius, OGLWidget::getYTop() + mRadius);
    glVertex2f(OGLWidget::getXLeft() + mRadius, OGLWidget::getYTop() + mRadius);
    //RECTANGULO INFERIOR
    glVertex2f(OGLWidget::getXLeft() + mRadius, OGLWidget::getYBottom() - mRadius);
    glVertex2f(OGLWidget::getXRight() - mRadius, OGLWidget::getYBottom() - mRadius);
    glVertex2f(OGLWidget::getXRight() - mRadius, OGLWidget::getYBottom());
    glVertex2f(OGLWidget::getXLeft() + mRadius, OGLWidget::getYBottom());
    glEnd();

    //CALCULO CANTIDAD DE VERTICES
    //std::ceil(2.0f*sqrt(radius)*(arc/PI)
    int steps = std::ceil(sqrt(mRadius));

#define PI 3.141592654f
#define PI_DIV_2 1.570796327f
#define PI_DIV_4 0.785398163f

    //CORNER SUPERIOR DERECHO
    drawArc(OGLWidget::getXRight() - mRadius, OGLWidget::getYTop() + mRadius, 0.0f, PI_DIV_2, steps);
    //CORNER SUPERIOR IZQUIERDO
    drawArc(OGLWidget::getXLeft() + mRadius, OGLWidget::getYTop() + mRadius, PI_DIV_2, PI_DIV_2, steps);
    //CORNER INFERIOR IZQUIERDO
    drawArc(OGLWidget::getXLeft() + mRadius, OGLWidget::getYBottom() - mRadius, PI, PI_DIV_2, steps);
    //CORNER INFERIOR DERECHO
    drawArc(OGLWidget::getXRight() - mRadius, OGLWidget::getYBottom() - mRadius, PI + PI_DIV_2, PI_DIV_2, steps);
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

    case OGL_WIDGET_MOVE:
    case OGL_WIDGET_SIZE:
        OGLWidget::onEvent(event);
        //NO ES NECESARIO SOLICITAR UN DRAW
        break;

    case OGL_WIDGET_REQUEST_EVENT_MASK:
        ret = OGLWidget::onEvent(event) | OGL_WIDGET_MOUSE_MOVE |
              OGL_WIDGET_MOUSE_LEAVE | OGL_WIDGET_FOCUS_RELEASE |
              OGL_WIDGET_MOUSE_CLICK_DOWN | OGL_WIDGET_DRAW |
              OGL_WIDGET_SIZE;
        break;
    }

    return ret;
}
