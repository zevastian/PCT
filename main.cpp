#include<iostream>
#include"OGLWindow.h"
#include"OGLWidget.h"
#include"OGLImage.h"
#include"OGLButton.h"
#include"OGLRoundedButton.h"
#include"OGLLabel.h"
#include"OGLScrollbar.h"
#include"OGLContainer.h"
#include"OGLUtils.h"
#include<string>
#include<jpeglib.h>
#include<setjmp.h>
#include<vector>
#include<stdexcept>
#include<fstream>


/*********************************************************************************************/
struct jpeg_error {
    jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};
typedef struct jpeg_error *jpeg_error_ptr;
/*********************************************************************************************/

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

void jpeg_error_func(j_common_ptr cinfo)
{
    jpeg_error_ptr err = (jpeg_error_ptr) cinfo->err;
    (*cinfo->err->output_message)(cinfo);
    longjmp(err->setjmp_buffer, 1);
}

bool decodeJPEGImage(std::vector<char> jpeg, unsigned int &width, unsigned int &height, std::vector<char> &bufferDest)
{
    jpeg_decompress_struct cinfo;
    jpeg_error jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = jpeg_error_func;

    if (!jpeg.size()) {
        return false;
    }

    FILE* infile = NULL;
    if (!(infile = fmemopen(jpeg.data(), jpeg.size(), "rb"))) {
        throw std::runtime_error("fmemopen failed");
    }

    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        throw std::runtime_error("setjmp return 1");
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    jpeg_read_header(&cinfo, TRUE);
    cinfo.dct_method = JDCT_DEFAULT;
    cinfo.out_color_space = JCS_EXT_BGRA;
    jpeg_start_decompress(&cinfo);

    if (!cinfo.output_width || !cinfo.output_height) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return false;
    }

    width = cinfo.output_width;
    height = cinfo.output_height;

    int pitch = cinfo.output_width*cinfo.output_components;
    bufferDest.resize(cinfo.output_height*pitch);

    unsigned char* buffer = (unsigned char*)bufferDest.data();
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, &buffer, 1);
        buffer += pitch;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    return true;
}

bool loadTexture(GLuint &texture, unsigned int &width, unsigned int& height, std::vector<char> &src)
{
    std::vector<char> dst;
    if (!decodeJPEGImage(src, width, height, dst)) {
        return false;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, dst.data());
    glGenerateMipmap (GL_TEXTURE_2D);

    return true;
}

void clickHello() {
    std::cout << "hola" << std::endl;
}

int main()
{
    OGLWindowDescription desc;
    desc.title = "PopcornTime++";
    desc.width = utils::getScreenWidth()*0.85f;
    desc.minWidth = utils::getScreenWidth()*0.45f;
    desc.maxWidth = utils::getScreenWidth();
    desc.height = utils::getScreenHeight()*0.85f;
    desc.minHeight = utils::getScreenHeight()*0.45f;
    desc.maxHeight = utils::getScreenHeight();
    desc.x = 0.5f*(utils::getDesktopWidth() - desc.width);
    desc.y = 0.5f*(utils::getDesktopHeight() - desc.height);

    OGLWindow oglwindow(desc);
    /**********************************************************************************/
    OGLXWindowHandle glxWindow;
    oglwindow.getNativeGLXWindow(glxWindow);
    OGLDisplayHandle display;
    oglwindow.getNativeDisplay(display);
    OGLFBConfig fbc;
    oglwindow.getNativeFBConfig(fbc);
    GLXContext context = NULL;
    context = glXCreateNewContext(display, fbc, GLX_RGBA_TYPE, 0, True);
    if (!context) {
        std::cout << "create gl context failed" << std::endl;
        return -1;
    }
    glXMakeContextCurrent(display, glxWindow, glxWindow, context);
    bool redraw = false;
    /**********************************************************************************/
    OGLWidgetDescription descWidget;
    descWidget.x.flag = OGLWidgetXFlag::OGL_ALIGN_LEFT;
    descWidget.y.flag = OGLWidgetYFlag::OGL_ALIGN_TOP;

    descWidget.width.value = 100.0f;
    descWidget.width.flag = OGLWidgetDimensionFlag::OGL_PERCENT;
    descWidget.height.value = 100.0f;
    descWidget.height.flag = OGLWidgetDimensionFlag::OGL_PERCENT;

    OGLContainer mainWidget(descWidget);
    /**********************************************************************************/
    OGLWidgetDescription descWidgetImage;
    descWidgetImage.x.flag = OGLWidgetXFlag::OGL_ALIGN_LEFT;
    descWidgetImage.y.flag = OGLWidgetYFlag::OGL_ALIGN_TOP;

    descWidgetImage.width.value = 100.0f;
    descWidgetImage.width.flag = OGLWidgetDimensionFlag::OGL_PERCENT;
    descWidgetImage.height.value = 100.0f;
    descWidgetImage.height.flag = OGLWidgetDimensionFlag::OGL_PERCENT;

    OGLImageDescription descImage;
    descImage.widget = descWidgetImage;
    descImage.image.scale = OGL_IMAGE_FILL;

    std::ifstream stream("img.jpg", std::ios::in | std::ios::binary);
    std::vector<char> buffer((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    loadTexture(descImage.texture.texture, descImage.texture.width, descImage.texture.height, buffer);
    std::shared_ptr<OGLIWidget> img1(new OGLImage(descImage));
    mainWidget.add(img1);
    /**********************************************************************************/

    OGLRoundedButtonDescription descButton;
    descButton.widget.x.value = 10.0f;
    descButton.widget.x.flag = OGLWidgetXFlag::OGL_RELATIVE_LEFT;
    descButton.widget.y.value = 10.0f;
    descButton.widget.y.flag = OGLWidgetYFlag::OGL_RELATIVE_BOTTOM;

    descButton.widget.width.value = 128.0f;
    descButton.widget.width.flag = OGLWidgetDimensionFlag::OGL_PX;
    descButton.widget.height.value = 48.0f;
    descButton.widget.height.flag = OGLWidgetDimensionFlag::OGL_PX;

    descButton.button.onClick = clickHello;
    descButton.button.radius = 6.0f;
    std::shared_ptr<OGLIWidget> btn1 (new OGLRoundedButton(descButton));
    mainWidget.add(btn1);
    /**********************************************************************************/
    OGLWidgetDescription descCont2;
    descCont2.x.flag = OGLWidgetXFlag::OGL_ALIGN_LEFT;
    descCont2.y.flag = OGLWidgetYFlag::OGL_ALIGN_TOP;

    descCont2.width.value = 33.3f;
    descCont2.width.flag = OGLWidgetDimensionFlag::OGL_PERCENT;
    descCont2.height.value = 100.0f;
    descCont2.height.flag = OGLWidgetDimensionFlag::OGL_PERCENT;

    std::shared_ptr<OGLContainer> labelContainer(new OGLContainer(descCont2));
    /**********************************************************************************/

    OGLLabelDescription descLabel;
    descLabel.widget.x.flag = OGLWidgetXFlag::OGL_ALIGN_CENTER_X;
    descLabel.widget.y.flag = OGLWidgetYFlag::OGL_ALIGN_CENTER_Y;

    descLabel.widget.width.value = 80.0f;
    descLabel.widget.width.flag = OGLWidgetDimensionFlag::OGL_PERCENT;
    descLabel.widget.height.value = 70.0f;
    descLabel.widget.height.flag = OGLWidgetDimensionFlag::OGL_PERCENT;

    descLabel.text.font = "Font/Roboto-Medium.ttf";
    descLabel.text.size = 16;
    descLabel.text.str = "In the near future, Major Motoko Kusanagi (Scarlett Johansson) is the first of her kind: A human saved from a terrible terrorist attack, who is cyber-enhanced to be a perfect soldier devoted to stopping the world's most dangerous criminals. When terrorism reaches a new level that includes the ability to hack into people's minds and control them, Major Kusanagi is uniquely qualified to stop it. As she prepares to face a new enemy, Major Kusanagi discovers that she has been lied to: her life was not saved, it was stolen. She will stop at nothing to recover her past, find out who did this to her and stop them before they do it to others. ";

    std::shared_ptr<OGLIWidget> lbl1(new OGLLabel(descLabel));
    labelContainer->add(lbl1);
    //PORQUE AFECTA LLAMARLO ANTES Y NO DESPUES DE AGREGARLE
    //EL LABEL?
    mainWidget.add(labelContainer);
    /**********************************************************************************/

    OGLWidgetEvent wEvent;
    wEvent.type = OGL_WIDGET_MOVE;
    wEvent.data.move.x = 0.0f;
    wEvent.data.move.y = 0.0f;
    mainWidget.onEvent(wEvent);
    oglwindow.show();
    OGLWindowEvent event;
    while (true) {

        while (!redraw || (redraw && oglwindow.pendingEvent())) {
            redraw = true;
            oglwindow.getEvent(event);
            switch (event.type) {

            case OGL_WINDOW_EXPOSE:
                redraw = true;
                break;

            case OGL_WINDOW_SIZE:
                glViewport(0, 0, OGL_WINDOW_SIZE_GET_WIDTH(event), OGL_WINDOW_SIZE_GET_HEIGHT(event));
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(0.0f, OGL_WINDOW_SIZE_GET_WIDTH(event), OGL_WINDOW_SIZE_GET_HEIGHT(event), 0.0f, 0.0f, 1.0f);
                wEvent.type = OGL_WIDGET_SIZE;
                wEvent.data.size.width = OGL_WINDOW_SIZE_GET_WIDTH(event);
                wEvent.data.size.height = OGL_WINDOW_SIZE_GET_HEIGHT(event);
                mainWidget.onEvent(wEvent);
                redraw = true;
                break;

            case OGL_WINDOW_CLOSE:
                glXMakeCurrent(display, 0, 0);
                glXDestroyContext(display, context);
                return 0;

            case OGL_WINDOW_MOUSE_MOVE:
                wEvent.type = OGL_WIDGET_MOUSE_MOVE;
                wEvent.data.mouse.x = OGL_WINDOW_MOUSE_GET_X(event);
                wEvent.data.mouse.y = OGL_WINDOW_MOUSE_GET_Y(event);
                mainWidget.onEvent(wEvent);
                redraw = true;
                //std::cout << "x: " << OGL_WINDOW_MOUSE_GET_X(event) << " y: " << OGL_WINDOW_MOUSE_GET_Y(event) << std::endl;
                break;

            case OGL_WINDOW_MOUSE_WHEEL:
                wEvent.type = OGL_WIDGET_MOUSE_WHEEL;
                wEvent.data.mouse.delta = OGL_WINDOW_MOUSE_GET_DELTA(event);
                //wEvent.data.mouse.y = OGL_WINDOW_MOUSE_GET_Y(event);
                mainWidget.onEvent(wEvent);
                redraw = true;
                //std::cout << "delta: " << std::to_string(OGL_WINDOW_MOUSE_GET_DELTA(event)) << std::endl;
                break;

            case OGL_WINDOW_MOUSE_CLICK_UP:
                wEvent.type = OGL_WIDGET_MOUSE_CLICK_UP;
                wEvent.data.mouse.x = OGL_WINDOW_MOUSE_GET_X(event);
                wEvent.data.mouse.y = OGL_WINDOW_MOUSE_GET_Y(event);
                mainWidget.onEvent(wEvent);
                redraw = true;
                //std::cout << "click up en x: " << OGL_WINDOW_MOUSE_GET_X(event) << " y: " << OGL_WINDOW_MOUSE_GET_Y(event) << std::endl;
                break;

            case OGL_WINDOW_MOUSE_CLICK_DOWN:
                wEvent.type = OGL_WIDGET_MOUSE_CLICK_DOWN;
                wEvent.data.mouse.x = OGL_WINDOW_MOUSE_GET_X(event);
                wEvent.data.mouse.y = OGL_WINDOW_MOUSE_GET_Y(event);
                mainWidget.onEvent(wEvent);
                redraw = true;
                //std::cout << "click down en x: " << OGL_WINDOW_MOUSE_GET_X(event) << " y: " << OGL_WINDOW_MOUSE_GET_Y(event) << std::endl;
                break;

            case OGL_WINDOW_FOCUS_SET:
                //std::cout << "focus set" << std::endl;
                break;

            case OGL_WINDOW_FOCUS_RELEASE:
                wEvent.type = OGL_WIDGET_FOCUS_RELEASE;
                mainWidget.onEvent(wEvent);
                redraw = true;
                //std::cout << "focus release" << std::endl;
                break;

            case OGL_WINDOW_MOUSE_ENTER:
                //std::cout << "enter" << std::endl;
                wEvent.type = OGL_WIDGET_MOUSE_ENTER;
                mainWidget.onEvent(wEvent);
                redraw = true;
                break;

            case OGL_WINDOW_MOUSE_LEAVE:
                //std::cout << "leave" << std::endl;
                wEvent.type = OGL_WIDGET_MOUSE_LEAVE;
                mainWidget.onEvent(wEvent);
                redraw = true;
                break;

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

//        glClear(GL_COLOR_BUFFER_BIT);
//        glClearColor(177/256.0f, 98/256.0f, 107/256.0f, 1.0f);
//        glClearColor(0.2, 0.2, .2, 1.0f);
        OGLWidgetEvent ev;
        ev.type = OGL_WIDGET_DRAW;
        mainWidget.onEvent(ev);
        glXSwapBuffers(display, glxWindow);
        redraw = false;
    }

    return 0;
}
