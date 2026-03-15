
#pragma once

#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

class IndexBuffer
{
public:
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    unsigned int getCount() const;

private:
    unsigned int m_renderID;
    unsigned int m_nCount;
};

#endif // !__INDEX_BUFFER_H__