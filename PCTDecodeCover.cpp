#include"PCTDecodeCover.h"
#include<jpeglib.h>
#include<setjmp.h>
#include<string.h>

struct jpeg_error {
    jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};
typedef struct jpeg_error *jpeg_error_ptr;

void jpeg_error_func(j_common_ptr cinfo)
{
    jpeg_error_ptr err = (jpeg_error_ptr) cinfo->err;
    (*cinfo->err->output_message)(cinfo);
    longjmp(err->setjmp_buffer, 1);
}

bool decodeJPEGImage(std::vector<char> jpeg, int &width, int &height, std::vector<char> &bufferDest)
{
    if (!jpeg.size()) {
        return false;
    }

    jpeg_error jerr;
    jpeg_decompress_struct cinfo;
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = jpeg_error_func;
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        return false;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, (unsigned char*)jpeg.data(), jpeg.size());

    jpeg_read_header(&cinfo, TRUE);
    cinfo.dct_method = JDCT_FASTEST;
    cinfo.out_color_space = JCS_EXT_BGRA;
    jpeg_start_decompress(&cinfo);

    if (!cinfo.output_width || !cinfo.output_height) {
        jpeg_destroy_decompress(&cinfo);
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
    return true;
}

PCTDecodeCover::PCTDecodeCover(std::vector<char> data, std::shared_ptr<PCTCancellationToken> token, unsigned int indx, PCTCommon common, int priority)
    :PCTWorkItem(priority), mData(data), mToken(token), mIndx(indx), mCommon(common)
{
    //NADA
}

void PCTDecodeCover::run()
{
    mToken->getMutex()->lock();
    if (!mToken->isCancelled()) {
        mCommon.mutexTextures->lock();
        if (decodeJPEGImage(mData, (*mCommon.textures.get())[mIndx].width, (*mCommon.textures.get())[mIndx].height, (*mCommon.textures.get())[mIndx].data)) {
            XClientMessageEvent event;
            memset(&event, 0, sizeof(XClientMessageEvent));
            event.type = ClientMessage;
            event.format = 32;
            event.data.s[0] = 1;
            event.data.s[1] = mIndx;
            XSendEvent(mCommon.display, mCommon.window, 0, 0, (XEvent*)&event);
            XFlush(mCommon.display);
        }
        mCommon.mutexTextures->unlock();
    }
    mToken->getMutex()->unlock();
}
