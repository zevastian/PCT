#include"OGLLabel.h"

//ATENCION
//TODA ESTA CLASE ES ALTAMENTE EXPERIMENTAL
OGLLabel::OGLLabel(OGLLabelDescription description) : OGLWidget(description.widget)
{

    int err = FT_Init_FreeType(&mFTLibrary);
    if (err) {
        throw std::runtime_error("FT_Init_FreeType failed");
    }

    err = FT_New_Face(mFTLibrary, description.text.font.c_str(), 0, &mFTFace);
    if (err) {
        FT_Done_FreeType(mFTLibrary);
        throw std::runtime_error("FT_New_Face failed");
    }

    FT_Set_Pixel_Sizes(mFTFace, 0, description.text.size);
    mDescription = description;
    //FALTAN CHEQUEAR CAMPOS
}

OGLLabel::~OGLLabel()
{
    if (mFTLibrary) {
        FT_Done_Face(mFTFace);
        FT_Done_FreeType(mFTLibrary);
    }
}

void OGLLabel::loadGlyph(char character)
{
    if (FT_Load_Char(mFTFace, character, FT_LOAD_RENDER)) {
        throw std::runtime_error("FT_Load_Char failed");
    }

    char* buffer = new char[4*mFTFace->glyph->bitmap.width*mFTFace->glyph->bitmap.rows];
    for (unsigned int y = 0; y < mFTFace->glyph->bitmap.rows; y++) {
        for (unsigned int x = 0; x < 4*mFTFace->glyph->bitmap.width; x +=4) {
            buffer[4*y*mFTFace->glyph->bitmap.width + x + 0] = 255;
            buffer[4*y*mFTFace->glyph->bitmap.width + x + 1] = 255;
            buffer[4*y*mFTFace->glyph->bitmap.width + x + 2] = 255;
            buffer[4*y*mFTFace->glyph->bitmap.width + x + 3] = mFTFace->glyph->bitmap.buffer[y*mFTFace->glyph->bitmap.width + x/4];
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,mFTFace->glyph->bitmap.width, mFTFace->glyph->bitmap.rows,
                 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);

    delete[] buffer;

    Glyph glyph;
    glyph.width = mFTFace->glyph->bitmap.width;
    glyph.height = mFTFace->glyph->bitmap.rows;
    glyph.advance = mFTFace->glyph->advance.x;
    glyph.advanceY = mFTFace->glyph->metrics.vertAdvance >> 6;
    glyph.bearingX = mFTFace->glyph->bitmap_left;
    glyph.bearingY = mFTFace->glyph->bitmap_top;
    glyph.cargado = true;
    glyph.texture = texture;

    mGlyphMap[character] = glyph;
}

bool OGLLabel::getWord(std::string text, unsigned int& indx, std::string& word)
{
    word = "";
    while (text[indx] != ' ' && indx < text.length()) {
        word.push_back(text[indx]);
        indx++;
    }
    if (text[indx] == ' ' && word == "") {
        word = " ";
        indx++;
    }
    if (indx >= text.length() && word == "") {
        return false;
    }

    return true;
}

void OGLLabel::drawText()
{
    std::string word;
    unsigned int indx = 0;

    struct {
        float left;
        float right;
        float top;
        float buttom;
    } rect;
    rect.left = OGLWidget::getXLeft();
    rect.right = OGLWidget::getXRight();
    rect.top = OGLWidget::getYTop();
    rect.buttom = OGLWidget::getYBottom();

    int left = rect.left;
    while (getWord(mDescription.text.str, indx, word)) {

        int width = 0;
        Glyph glyph;
        for (unsigned int i = 0; i < word.length(); i++) {
            glyph = mGlyphMap[word[i]];
            if (!glyph.cargado) {
                loadGlyph(word[i]);
                glyph = mGlyphMap[word[i]];
            }

            width += (glyph.advance >> 6);
        }

        if (rect.left + width > rect.right) {
            if (word[0] == ' ') {
                continue;
            }

            rect.top += glyph.advanceY;
            rect.left = left;
        }

        if (rect.top > rect.buttom) {
            break;
        }

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        for (unsigned int i = 0; i < word.length(); i++) {
            glyph = mGlyphMap[word[i]];
            glBindTexture(GL_TEXTURE_2D, glyph.texture);

            int X = rect.left + glyph.bearingX;
            int Y = rect.top - glyph.bearingY;

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(X, Y);

            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(X, Y + glyph.height);

            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(X + glyph.width, Y + glyph.height);

            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(X + glyph.width, Y);
            glEnd();

            rect.left += (glyph.advance >> 6);
        }
    }
}

int OGLLabel::onEvent(OGLWidgetEvent event)
{
    int ret = OGL_WIDGET_RET_NONE;
    switch (event.type) {

    case OGL_WIDGET_MOVE:
    case OGL_WIDGET_SIZE:
        OGLWidget::onEvent(event);
        //NO INFORMAR DRAW
        break;

    case OGL_WIDGET_DRAW:
        drawText();
        break;

    case OGL_WIDGET_REQUEST_EVENT_MASK:
        ret = OGLWidget::onEvent(event) | OGL_WIDGET_SIZE |
              OGL_WIDGET_DRAW;
        break;
    }

    return ret;
}
