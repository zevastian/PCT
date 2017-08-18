#ifndef _OGL_IWIDGET_H_
#define _OGL_IWIDGET_H_

#define OGL_WIDGET_SIZE 0X0001
#define OGL_WIDGET_MOVE 0X0002
#define OGL_WIDGET_DRAW 0X0004
#define OGL_WIDGET_ANIMATE 0X0008

#define OGL_WIDGET_FOCUS_RELEASE 0X0010

#define OGL_WIDGET_MOUSE_MOVE 0X0020
#define OGL_WIDGET_MOUSE_CLICK_UP 0X0040
#define OGL_WIDGET_MOUSE_CLICK_DOWN 0X0080
#define OGL_WIDGET_MOUSE_WHEEL 0X0100
#define OGL_WIDGET_MOUSE_ENTER 0X0200
#define OGL_WIDGET_MOUSE_LEAVE 0X0400

#define OGL_WIDGET_KEY_UP 0X0800
#define OGL_WIDGET_KEY_DOWN 0X1000
#define OGL_WIDGET_KEY_CHAR 0X2000

#define OGL_WIDGET_REQUEST_EVENT_MASK 0X4000

#define OGL_WIDGET_CLOSE 0X8000

struct OGLWidgetEvent {
    int type;
    union {
        struct {
            unsigned int width;
            unsigned int height;
        } size;
        struct {
            int x;
            int y;
        } move;
        struct {
            int x;
            int y;
            char delta;
        } mouse;
        struct {
            int code;
            char ch;
        } key;
    } data;
};

#define OGL_WIDGET_SIZE_GET_WIDTH(event) (event.data.size.width)
#define OGL_WIDGET_SIZE_GET_HEIGHT(event) (event.data.size.height)
#define OGL_WIDGET_MOVE_GET_X(event) (event.data.move.x)
#define OGL_WIDGET_MOVE_GET_Y(event) (event.data.move.y)

#define OGL_WIDGET_MOUSE_GET_X(event) (event.data.mouse.x)
#define OGL_WIDGET_MOUSE_GET_Y(event) (event.data.mouse.y)
#define OGL_WIDGET_MOUSE_GET_DELTA(event) (event.data.mouse.delta)

#define OGL_WIDGET_KEY_GET_CODE(event) (event.data.key.code)
#define OGL_WIDGET_KEY_GET_CHAR(event) (event.data.key.ch)

class OGLIWidget
{
public:
    /**/
    virtual int onEvent(OGLWidgetEvent event) = 0;
};

#endif
