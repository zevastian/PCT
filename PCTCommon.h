#include<mutex>
#include<vector>
#include<X11/Xlib.h>

#ifndef _PCTCOMMON_H_
#define _PCTCOMMON_H_

struct OGLTexture {
    unsigned int text;
    std::vector<char> data;
    int width;
    int height;
    bool available;
    bool animate;
    float alpha;
};

struct PCTCommon {
    std::shared_ptr<std::mutex> mutexTextures;
    std::shared_ptr<std::vector<OGLTexture>> textures;
    Display* display;
    Window window;
};


#endif // _PCTCOMMON_H_
