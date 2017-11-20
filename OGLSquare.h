#include"OGLWidget.h"

#ifndef _OGL_SQUARE_H_
#define _OGL_SQUARE_H_

struct OGLSquareDescription {
    OGLWidgetDescription widget;
    struct {
        float r;
        float g;
        float b;
        float a;
    } color;
};

//DUDO DE QUE ESTA CLASE EXISTA EN EL FUTURO
//TAL VEZ LOS WIDGET TENDRAN UN COLOR DE FONDO
class OGLSquare : public OGLWidget
{
private:
    OGLSquareDescription mDescription;

public:
    /**/
    OGLSquare(OGLSquareDescription description);
    /**/
    ~OGLSquare();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
