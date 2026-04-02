#include "texture.h"
#include "error.h"

#include <stb_image.h>

#include <iostream>

Texture::Texture(const std::string& path)
    : m_renderID(0)
    , m_filePath(path)
    , m_localBuffer(nullptr)
    , m_width(0)
    , m_height(0)
    , m_BPP(0)
{
    /* The origin of an image is typically at the top-left corner, 
       whereas in OpenGL it is at the bottom-left corner */
    stbi_set_flip_vertically_on_load(1);

    m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4 /*RGBA*/);
    if (!m_localBuffer)
    {
        std::cerr << "ERROR::TEXTURE::Failed to load texture: " << path << std::endl;
        return;
    }

    GLCALL(glGenTextures(1, &m_renderID));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_renderID));

    /* Some settings */
    /* Use linear interpolation when scaling images */
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    /* When image coordinates exceed bounds, stretch edge colours */
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_localBuffer)
    {
        stbi_image_free(m_localBuffer);
        m_localBuffer = nullptr;
    }
}

Texture::~Texture()
{
    GLCALL(glDeleteTextures(1, &m_renderID));
}

void Texture::bind(unsigned int slot /*= 0*/) const
{
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_renderID));
}

void Texture::unbind() const
{
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

const std::string& Texture::getPath() const
{
    return m_filePath;
}

int Texture::getWidth() const
{
    return m_width;
}

int Texture::getHeight() const
{
    return m_height;
}

bool Texture::isValid() const
{
    return m_renderID != 0;
}