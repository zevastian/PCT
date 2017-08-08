#include<iostream>
#include"OGLWindow.h"
#include<string>

Display* openDisplay()
{
    return XOpenDisplay(NULL);
}

unsigned int desktopWidth()
{
    Display *display = openDisplay();
    int width = DefaultScreenOfDisplay(display)->width;
    XCloseDisplay(display);
    return width;
}

unsigned int desktopHeight()
{
    Display *display = openDisplay();
    int height = DefaultScreenOfDisplay(display)->height;
    XCloseDisplay(display);
    return height;
}

unsigned int screenWidth()
{
    Display *display = openDisplay();
    int width = DefaultScreenOfDisplay(display)->width;
    XCloseDisplay(display);
    return width;
}

unsigned int screenHeight()
{
    Display *display = openDisplay();
    int height = DefaultScreenOfDisplay(display)->height;
    XCloseDisplay(display);
    return height;
}
//
//void getNetWMState(unsigned char*& properties)
//{
////    Atom type;
////    int format = 0;
////    unsigned long numProperties = 0;
////    unsigned long bytesAfter = 0;
////    unsigned char* properties = NULL;
////    Atom netWMState = XInternAtom(mDisplay, "_NET_WM_STATE", False);
////    int screen = DefaultScreen(mDisplay);
////    Window root = RootWindow(mDisplay, screen);
////
////    if (XGetWindowProperty(mDisplay, root, netWMState, 0, LONG_MAX, False,
////            XA_ATOM, &type, &format, &numProperties, &bytesAfter, &properties) == Success) {
////
////
////
////
////    }
//
//
//
//
//
//
//}
//
//void changeNetWMState()
//{
//
//
//
//}


void printState(OGLWindow& window)
{
    int state = window.getState();
    std::string str;

    if (state & OGL_STATE_FULLSCREEN) {
        str += "fullscreen, ";
    }

    if (state & OGL_STATE_NORMAL) {
        str += "normal, ";
    }
    if (state & OGL_STATE_MAXIMIZED_HORZ) {
        str += "max_horz, ";
    }
    if (state & OGL_STATE_MAXIMIZED_VERT) {
        str += "max_ver, ";
    }
    std::cout << str << std::endl;
}

int main()
{
    OGLWindowDescription desc;
    desc.title = "PopcornTime++";
    desc.width = screenWidth()*0.85f;
    desc.minWidth = screenWidth()*0.45f;
    desc.maxWidth = screenWidth();
    desc.height = screenHeight()*0.85f;
    desc.minHeight = screenHeight()*0.45f;
    desc.maxHeight = screenHeight();
    desc.x = 0.5f*(desktopWidth() - desc.width);
    desc.y = 0.5f*(desktopHeight() - desc.height);

    OGLWindow oglwindow(desc);


    /**********************************************************************************/
    OGLXWindowHandle glxWindow;
    oglwindow.getNativeGLXWindow(glxWindow);
    OGLDisplayHandle display;
    oglwindow.getNativeDisplay(display);
    OGLFBConfig fbc;
    oglwindow.getNativeFBConfig(fbc);


    /**********************************************************************************/
    GLXContext context = NULL;
    context = glXCreateNewContext(display, fbc, GLX_RGBA_TYPE, 0, True);
    if (!context) {
        std::cout << "create gl context failed" << std::endl;
        return -1;
    }
    glXMakeContextCurrent(display, glxWindow, glxWindow, context);
    /**********************************************************************************/



//    OGLWindowHandle window;
//                oglwindow.getNativeWindow(window);
//    Atom atom = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
//  XChangeProperty(
//      display,
//      window,
//      XInternAtom(display, "_NET_WM_STATE", False),
//      XA_ATOM, 32, PropModeReplace, (unsigned char *)&atom, 1);



    oglwindow.show();


    OGLWindowEvent event;
    while (true) {

        oglwindow.getEvent(event);
        switch (event.type) {

        case OGL_WINDOW_EXPOSE:
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(177/256.0f, 98/256.0f, 107/256.0f, 1.0f);
            glXSwapBuffers(display, glxWindow);
            break;

        case OGL_WINDOW_SIZE:
            glViewport(0, 0, OGL_WINDOW_SIZE_GET_WIDTH(event), OGL_WINDOW_SIZE_GET_HEIGHT(event));
            break;

        case OGL_WINDOW_CLOSE:
            glXMakeCurrent(display, 0, 0);
            glXDestroyContext(display, context);
            return 0;

//        case OGL_WINDOW_MOUSE_MOVE:
//            std::cout << "x: " << OGL_WINDOW_MOUSE_GET_X(event) << " y: " << OGL_WINDOW_MOUSE_GET_Y(event) << std::endl;
//            break;
//
//        case OGL_WINDOW_MOUSE_WHEEL:
//            std::cout << "delta: " << std::to_string(OGL_WINDOW_MOUSE_GET_DELTA(event)) << std::endl;
//            break;
//
//        case OGL_WINDOW_MOUSE_CLICK_UP:
//            std::cout << "click up en x: " << OGL_WINDOW_MOUSE_GET_X(event) << " y: " << OGL_WINDOW_MOUSE_GET_Y(event) << std::endl;
//            break;
//
//        case OGL_WINDOW_MOUSE_CLICK_DOWN:
//            std::cout << "click down en x: " << OGL_WINDOW_MOUSE_GET_X(event) << " y: " << OGL_WINDOW_MOUSE_GET_Y(event) << std::endl;
//            break;
//
//        case OGL_WINDOW_FOCUS_SET:
//            std::cout << "focus set" << std::endl;
//            break;
//
//        case OGL_WINDOW_FOCUS_RELEASE:
//            std::cout << "focus release" << std::endl;
//            break;
//
//        case OGL_WINDOW_MOUSE_ENTER:
//            std::cout << "enter" << std::endl;
//            break;
//
//        case OGL_WINDOW_MOUSE_LEAVE:
//            std::cout << "leave" << std::endl;
//            break;

        case OGL_WINDOW_KEY_UP:
//            std::cout << "code up: " <<  OGL_WINDOW_KEY_GET_CODE(event) << std::endl;
            break;

        case OGL_WINDOW_KEY_DOWN:
//            std::cout << "code down: " <<  OGL_WINDOW_KEY_GET_CODE(event) << std::endl;
                if (OGL_WINDOW_KEY_GET_CODE(event) == 41) {
                    int state = oglwindow.getState();
                    if (state == OGL_STATE_NONE) {
                        break;
                    }
                    if (!(state & OGL_STATE_FULLSCREEN)) {
                        oglwindow.fullscreen();
                    } else {
                        oglwindow.restore();
                    }
                }
            break;

        }
    }

    return 0;
}
