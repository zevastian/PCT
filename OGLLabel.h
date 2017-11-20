#include<ft2build.h>
#include<epoxy/gl.h>
#include<map>
#include FT_FREETYPE_H
#include"OGLWidget.h"

#ifndef _OGL_LABEL_H_
#define _OGL_LABEL_H_

struct OGLLabelDescription {
    OGLWidgetDescription widget;
    struct {
        //FALTARIA ALINEACION Y COLOR
        std::string str;
        std::string font;
        int size;
    } text;
};

class OGLLabel : public OGLWidget
{
private:
    struct Glyph {
        int width;
        int height;
        int bearingX;
        int bearingY;
        int advance;
        int advanceY;
        bool cargado;
        GLuint texture;
        Glyph ()
        {
            cargado = false;
        }
    };

    FT_Library mFTLibrary;
    FT_Face mFTFace;
    std::map<char,Glyph> mGlyphMap;
    OGLLabelDescription mDescription;
    void loadGlyph(char character);
    bool getWord(std::string text, unsigned int& indx, std::string& word);
    void drawText();

public:
    /**/
    OGLLabel(OGLLabelDescription description);
    /**/
    ~OGLLabel();

    /**/
    int onEvent(OGLWidgetEvent event);
};

#endif
