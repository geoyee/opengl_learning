#include "vertex_array.h"
#include "error.h"

#pragma warning(disable : 4312)

static unsigned int getSizeOfType(unsigned int type)
{
    switch (type)
    {
        case GL_FLOAT :
            return sizeof(GLfloat);
        case GL_UNSIGNED_INT :
            return sizeof(GLuint);
        case GL_UNSIGNED_BYTE :
            return sizeof(GLubyte);
        default :
            return 0;
    }
}

VertexArrary::VertexArrary()
{
    GLCALL(glGenVertexArrays(1, &m_renderID));
}

VertexArrary::~VertexArrary()
{
    GLCALL(glDeleteVertexArrays(1, &m_renderID));
}

void VertexArrary::bind() const
{
    GLCALL(glBindVertexArray(m_renderID));
}

void VertexArrary::unbind() const
{
    GLCALL(glBindVertexArray(0));
}

void VertexArrary::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    size_t element_size = elements.size();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < element_size; ++i)
    {
        const VertexBufferElement& element = elements[i];
        GLCALL(glEnableVertexAttribArray(i));
        GLCALL(glVertexAttribPointer(i,
                                     element.count,
                                     element.type,
                                     element.normalized,
                                     layout.getStride(),
                                     reinterpret_cast<const void *>(offset)));
        offset += element.count * getSizeOfType(element.type);
    }
}