#pragma once

namespace nimo{
class IndexBuffer
{
public:
    IndexBuffer(unsigned int* indices, unsigned int indexCount);
    ~IndexBuffer();
    void bind();
    void unbind();
    unsigned int count();

private:
    unsigned int m_id;
    unsigned int m_count;
};
};