#pragma once

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>

class Texture
{
public:
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    const std::string& getPath() const;
    int getWidth() const;
    int getHeight() const;
    bool isValid() const;

private:
    unsigned int m_renderID;
    std::string m_filePath;
    unsigned char *m_localBuffer;
    int m_width, m_height, m_BPP;
};

#endif // !__TEXTURE_H__