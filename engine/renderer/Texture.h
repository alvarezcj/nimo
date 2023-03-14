#pragma once

#include <string>
#include "assets/Asset.h"

namespace nimo{
class Texture : public Asset
{
public:
    Texture(unsigned int width, unsigned int height, void* data);
    Texture(const std::string& path);
    ~Texture();
    void bind(unsigned int slot);
    unsigned int GetInternalId() {return m_id;}

    AssetType Type() const { return AssetType::Texture; }
    static AssetType StaticType(){return AssetType::Texture;}

    inline unsigned int Width(){return m_width;}
    inline unsigned int Height(){return m_height;}

private:
    unsigned int m_id;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_internalFormat;
    unsigned int m_dataFormat;
};
};