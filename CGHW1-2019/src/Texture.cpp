#include "Texture.h"
#include "lodepng.h"
#include <cstdint>

// Reference https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexParameter.xhtml

static GLenum WrapModeOpenGLMapping[] =
{ GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };

static GLenum FilterModeOpenGLMapping[] =
{ GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_NEAREST, 
GL_LINEAR_MIPMAP_NEAREST };

static GLenum ColorTypeOpenGLMapping[] =
{ GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 };

Texture2D::Texture2D(uint32_t width, uint32_t height, ColorType type)
    : m_width(width), m_height(height)
{
    //產生texture name, 給予流水號(ID)
    glGenTextures(1, &m_id);
    //綁定要處理的空間
    glBindTexture(GL_TEXTURE_2D, m_id);
    //void glTexStorage2D( GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
    glTexStorage2D(GL_TEXTURE_2D, 1, ColorTypeOpenGLMapping[static_cast<uint32_t>(type)], width, height);
    //glTexImage2D(GL_TEXTURE_2D, 0, ColorTypeOpenGLMapping[static_cast<uint32_t>(type)], width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    m_mipmap = false;
    setFilter(FilterMode::eNearest, FilterMode::eNearest);
    setWrap(WrapMode::eRepeat, WrapMode::eRepeat);
}

Texture2D Texture2D::LoadFromFile(const std::string & pngfile)
{
    // load png and decode into raw data
    std::vector<uint8_t> raw_data;
    uint32_t width=0, height=0;
    lodepng::decode(raw_data, width, height, pngfile);
    if(width==0 || height==0){
        return Texture2D(0U);
    }

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    // upload!!
    //void glTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels)
    //定義一個2D的texture image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw_data.data());
    Texture2D ret(id);
    ret.m_mipmap = false;
    ret.setFilter(FilterMode::eNearest, FilterMode::eNearest);
    ret.setWrap(WrapMode::eRepeat, WrapMode::eRepeat);
    //設定寬/高
    ret.m_width = width;
    ret.m_height = height;
    return ret;
}

void Texture2D::bindToChannel(GLuint channel)
{
    //激活texture unit並綁定texture目標
    glActiveTexture(GL_TEXTURE0+channel);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::setWrap(WrapMode s, WrapMode t, WrapMode r)
{
    //調整texture的參數
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapModeOpenGLMapping[static_cast<uint32_t>(s)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapModeOpenGLMapping[static_cast<uint32_t>(t)]);
    // r is meaning less in texture2d
}

void Texture2D::release()
{
    glDeleteTextures(1, &m_id);
}

void Texture2D::setFilter(FilterMode min, FilterMode mag)
{
    glBindTexture(GL_TEXTURE_2D, m_id);
    // if mipmapping is required, generate it
    if (min >= FilterMode::eNearestMipmapLinear) {
        glGenerateMipmap(GL_TEXTURE_2D);
        m_mipmap = true;
    }
    // check parameter..
    if (mag >= FilterMode::eNearestMipmapLinear) {
        mag = FilterMode::eNearest;
    }
    //GL_TEXTURE_MIN_FILTER表示設定材質影像需要放大時該如何處理
    //GL_TEXTURE_MAG_FILTER表示設定材質影像需要縮小時該如何處理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterModeOpenGLMapping[static_cast<uint32_t>(min)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterModeOpenGLMapping[static_cast<uint32_t>(mag)]);
}

bool Texture2D::hasMipmap()
{
    return m_mipmap;
}

Texture2D::Texture2D(GLuint id)
    : ITexture(), m_id(id)
{
}